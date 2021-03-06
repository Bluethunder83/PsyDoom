#include "spu.h"
#include <fmt/core.h>
#include <imgui.h>
#include <vector>
#include "device/spu/spu.h"
#include "system.h"
#include <SDL.h>

namespace ImGui {
template <typename... Args>
void Fmt(const char* fmt, Args... args) {
    ImGui::TextUnformatted(fmt::format(fmt, args...).c_str());
}
};  // namespace ImGui

namespace gui::debug {
using namespace spu;

void channelsInfo(spu::SPU* spu, bool parseValues) {
    const int COL_NUM = 12;
    float columnsWidth[COL_NUM] = {0};
    int n = 0;

    auto column = [&](const std::string& str) {
        ImVec2 size = ImGui::CalcTextSize(str.c_str());
        float width = std::max(size.x + 8.f, 48.f);
        if (width > columnsWidth[n]) columnsWidth[n] = width;
        ImGui::TextUnformatted(str.c_str());
        ImGui::NextColumn();
        if (++n >= COL_NUM) n = 0;
    };

    auto mapState = [](Voice::State state) -> std::string {
        switch (state) {
            case Voice::State::Attack: return "A   ";
            case Voice::State::Decay: return " D  ";
            case Voice::State::Sustain: return "  S ";
            case Voice::State::Release: return "   R";
            case Voice::State::Off:
            default: return "    ";
        }
    };

    auto adsrInfo = [](ADSR adsr) -> std::string {
        std::string s;

        for (int i = 0; i < 4; i++) {
            Envelope e;
            if (i == 0) {
                e = adsr.attack();
                s += "Attack:\n";
            } else if (i == 1) {
                e = adsr.decay();
                s += "Decay:\n";
            } else if (i == 2) {
                e = adsr.sustain();
                s += "Sustain:\n";
            } else if (i == 3) {
                e = adsr.release();
                s += "Release:\n";
            }

            s += fmt::format("Step:      {}\n", e.getStep());
            s += fmt::format("Shift:     {}\n", e.shift);
            s += fmt::format("Direction: {}\n", e.direction == Envelope::Direction::Decrease ? "Decrease" : "Increase");
            s += fmt::format("Mode:      {}\n", e.mode == Envelope::Mode::Exponential ? "Exponential" : "Linear");
            s += fmt::format("Level:     {}\n\n", e.level);
        }

        return s;
    };

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::Columns(COL_NUM, nullptr, false);

    column(" ");
    column("Ch");
    column("State");
    column("VolL");
    column("VolR");
    column("ADSR Vol");
    column("Sample rate");
    column("Current addr");
    column("Start addr");
    column("Repeat addr");
    column("ADSR");
    column("Flags");
    for (int i = 0; i < spu::SPU::VOICE_COUNT; i++) {
        auto& v = spu->voices[i];

        if (v.state == Voice::State::Off) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
        }

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        if (ImGui::Checkbox(fmt::format("##enabled{}", i + 1).c_str(), &v.enabled)) {
            if (ImGui::GetIO().KeyShift) {
                for (int j = 0; j < spu::SPU::VOICE_COUNT; j++) {
                    if (j != i) spu->voices[j].enabled = false;
                }
                v.enabled = true;
            }
            if (ImGui::GetIO().KeyCtrl) {
                for (int j = 0; j < spu::SPU::VOICE_COUNT; j++) {
                    if (j != i) spu->voices[j].enabled = true;
                }
                v.enabled = false;
            }
        }
        ImGui::PopStyleVar();
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted("Shift + click - mute other channels");
            ImGui::TextUnformatted("Ctrl  + click - play other channels");
            ImGui::EndTooltip();
        }
        columnsWidth[n++] = ImGui::GetItemRectSize().x + 4.f;
        ImGui::NextColumn();

        column(fmt::format("{:2d}", i + 1));

        column(mapState(v.state));
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImU32 frameColor = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        if (v.state == Voice::State::Off) {
            frameColor = ImGui::GetColorU32(ImVec4(0.4f, 0.4f, 0.4f, .25f));
        }
        int barHeight = 12;  // TODO: Do not hardcode!
        auto bar = [&](float w) {
            ImVec2 src = ImGui::GetCursorScreenPos();
            drawList->AddRectFilled(src, ImVec2(src.x + ImGui::GetColumnWidth(n) * w, src.y + barHeight), frameColor);
        };
        if (parseValues) {
            bar(intToFloat(v.volume.getLeft()));
            column(fmt::format("{:.0f}", intToFloat(v.volume.getLeft()) * 100.f));

            bar(intToFloat(v.volume.getRight()));
            column(fmt::format("{:.0f}", intToFloat(v.volume.getRight()) * 100.f));

            bar(intToFloat(v.adsrVolume._reg));
            column(fmt::format("{:.0f}", (v.adsrVolume._reg / static_cast<float>(0x7fff)) * 100.f));
        } else {
            column(fmt::format("{:04x}", v.volume.left));
            column(fmt::format("{:04x}", v.volume.right));
            column(fmt::format("{:04x}", v.adsrVolume._reg));
        }

        if (parseValues) {
            column(fmt::format("{:5.0f} Hz", v.sampleRate._reg / 4096.f * 44100.f));
        } else {
            column(fmt::format("{:04x}", v.sampleRate._reg));
        }
        ImVec2 src = ImGui::GetCursorScreenPos();

        float startP = v.startAddress._reg * 8 / (float)spu::SPU::RAM_SIZE;
        float currP = v.currentAddress._reg * 8 / (float)spu::SPU::RAM_SIZE;
        float repeatP = v.repeatAddress._reg * 8 / (float)spu::SPU::RAM_SIZE;
        drawList->AddRectFilled(ImVec2(src.x + ImGui::GetColumnWidth(n) * startP, src.y),
                                ImVec2(src.x + ImGui::GetColumnWidth(n) * repeatP, src.y + barHeight), frameColor);

        ImU32 lineColor = ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1.f));
        drawList->AddLine(ImVec2(src.x + ImGui::GetColumnWidth(n) * currP, src.y),
                          ImVec2(src.x + ImGui::GetColumnWidth(n) * currP, src.y + barHeight), lineColor);

        column(fmt::format("{:04x}", v.currentAddress._reg));
        column(fmt::format("{:04x}", v.startAddress._reg));
        column(fmt::format("{:04x}", v.repeatAddress._reg));
        column(fmt::format("{:08x}", v.adsr._reg));
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(adsrInfo(v.adsr).c_str());
            ImGui::EndTooltip();
        }

        column(fmt::format("{} {} {}",
                           v.reverb ? "Reverb" : "",                     //
                           v.mode == Voice::Mode::Noise ? "Noise" : "",  //
                           v.pitchModulation ? "PitchModulation" : "")   //
        );

        ImGui::PopStyleColor();
    }

    for (int c = 0; c < COL_NUM; c++) {
        ImGui::SetColumnWidth(c, columnsWidth[c]);
    }

    ImGui::Columns(1);
    ImGui::PopFont();
    ImGui::PopStyleVar();
}

void reverbInfo(spu::SPU* spu) {
    const int width = 8;
    const int height = 4;
    const std::array<const char*, 32> registerDescription = {
        {"dAPF1   - Reverb APF Offset 1",
         "dAPF2   - Reverb APF Offset 2",
         "vIIR    - Reverb Reflection Volume 1",
         "vCOMB1  - Reverb Comb Volume 1",
         "vCOMB2  - Reverb Comb Volume 2",
         "vCOMB3  - Reverb Comb Volume 3",
         "vCOMB4  - Reverb Comb Volume 4",
         "vWALL   - Reverb Reflection Volume 2",
         "vAPF1   - Reverb APF Volume 1",
         "vAPF2   - Reverb APF Volume 2",
         "mLSAME  - Reverb Same Side Reflection Address 1 Left",
         "mRSAME  - Reverb Same Side Reflection Address 1 Right",
         "mLCOMB1 - Reverb Comb Address 1 Left",
         "mRCOMB1 - Reverb Comb Address 1 Right",
         "mLCOMB2 - Reverb Comb Address 2 Left",
         "mRCOMB2 - Reverb Comb Address 2 Right",
         "dLSAME  - Reverb Same Side Reflection Address 2 Left",
         "dRSAME  - Reverb Same Side Reflection Address 2 Right",
         "mLDIFF  - Reverb Different Side Reflect Address 1 Left",
         "mRDIFF  - Reverb Different Side Reflect Address 1 Right",
         "mLCOMB3 - Reverb Comb Address 3 Left",
         "mRCOMB3 - Reverb Comb Address 3 Right",
         "mLCOMB4 - Reverb Comb Address 4 Left",
         "mRCOMB4 - Reverb Comb Address 4 Right",
         "dLDIFF  - Reverb Different Side Reflect Address 2 Left",
         "dRDIFF  - Reverb Different Side Reflect Address 2 Right",
         "mLAPF1  - Reverb APF Address 1 Left",
         "mRAPF1  - Reverb APF Address 1 Right",
         "mLAPF2  - Reverb APF Address 2 Left",
         "mRAPF2  - Reverb APF Address 2 Right",
         "vLIN    - Reverb Input Volume Left",
         "vRIN    - Reverb Input Volume Right"},
    };

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::Text("Base: 0x%08x", spu->reverbBase._reg * 8);
    ImGui::Text("Current: 0x%08x", spu->reverbCurrentAddress);

    ImGui::Columns(8, nullptr, false);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = y * width + x;
            auto str = fmt::format("{:04x}", spu->reverbRegisters[i]._reg);
            ImGui::TextUnformatted(str.c_str());

            ImVec2 size = ImGui::CalcTextSize(str.c_str());
            ImGui::SetColumnWidth(x, size.x + 8.f);

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted(registerDescription[i]);
                ImGui::EndTooltip();
            }

            ImGui::NextColumn();
        }
    }

    ImGui::Columns(1);
    ImGui::PopStyleVar();
}

void registersInfo(spu::SPU* spu) {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::Text("Main   volume: %08x", spu->mainVolume._reg);
    ImGui::Text("CD     volume: %08x", spu->cdVolume._reg);
    ImGui::Text("Ext    volume: %08x", spu->extVolume._reg);
    ImGui::Text("Reverb volume: %08x", spu->reverbVolume._reg);
    ImGui::Fmt("Control: 0x{:04x}  {:10s} {:4s} {:8s} {:3s} {:6s} {:9s}",  //
               spu->control._reg,                                          //
               spu->control.spuEnable ? "SPU enable" : "",                 //
               spu->control.unmute ? "" : "Mute",                          //
               spu->control.cdEnable ? "Audio CD" : "",                    //
               spu->control.irqEnable ? "IRQ" : "",                        //
               spu->control.masterReverb ? "Reverb" : "",                  //
               spu->control.cdReverb ? "CD_Reverb" : ""                    //
    );
    ImGui::Fmt("Status:  0x{:04x}  CaptureBuffer={:6s} {:8s} {:8s} {:8s} {:9s} {:3s}",  //
               spu->status._reg,                                                        //
               spu->status.captureBufferHalf ? "Second" : "First",                      //
               spu->status.dmaBusy ? "DMA_Busy" : "",                                   //
               spu->status.dmaReadRequest ? "DMA_Rreq" : "",                            //
               spu->status.dmaWriteRequest ? "DMA_Wreq" : "",                           //
               spu->status.dmaReadWriteRequest ? "DMA_RWreq" : "",                      //
               spu->status.irqFlag ? "IRQ" : ""                                         //
    );

    ImGui::Text("IRQ Address: 0x%08x", spu->irqAddress._reg);
    ImGui::PopStyleVar();
}

void renderSamples(spu::SPU* spu) {
    std::vector<float> samples;
    for (auto s : spu->audioBuffer) {
        samples.push_back((float)s / (float)0x7fff);
    }

    ImGui::PlotLines("Preview", samples.data(), (int)samples.size(), 0, nullptr, -1.0f, 1.0f, ImVec2(400, 80));
}

void SPU::spuWindow(spu::SPU* spu) {
    const auto treeFlags = ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen;
    static bool parseValues = true;

    ImGui::Begin("SPU", &spuWindowOpen);

    if (ImGui::TreeNodeEx("Channels", treeFlags)) channelsInfo(spu, parseValues);
    if (ImGui::TreeNodeEx("Reverb", treeFlags)) reverbInfo(spu);
    if (ImGui::TreeNodeEx("Registers", treeFlags)) registersInfo(spu);

    ImGui::Checkbox("Parse values", &parseValues);
    renderSamples(spu);
    ImGui::End();
}

void SPU::displayWindows(System* sys) {
    if (spuWindowOpen) spuWindow(sys->spu.get());
}
};  // namespace gui::debug