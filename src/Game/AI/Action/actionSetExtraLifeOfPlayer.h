#pragma once

#include "KingSystem/ActorSystem/actAiAction.h"

namespace uking::action {

class SetExtraLifeOfPlayer : public ksys::act::ai::Action {
    SEAD_RTTI_OVERRIDE(SetExtraLifeOfPlayer, ksys::act::ai::Action)
public:
    explicit SetExtraLifeOfPlayer(const InitArg& arg);
    ~SetExtraLifeOfPlayer() override;

    bool init_(sead::Heap* heap) override;
    void enter_(ksys::act::ai::InlineParamPack* params) override;
    void leave_() override;
    void loadParams_() override;

protected:
    void calc_() override;

    // dynamic_param at offset 0x20
    int* mValue_d{};
    // dynamic_param at offset 0x28
    int* mProcessingMode_d{};
};

}  // namespace uking::action