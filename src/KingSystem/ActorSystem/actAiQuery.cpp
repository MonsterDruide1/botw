#include "KingSystem/ActorSystem/actAiQuery.h"
#include "KingSystem/ActorSystem/actActorParam.h"
#include "KingSystem/ActorSystem/actAiRoot.h"
#include "KingSystem/Resource/resResourceAIProgram.h"

namespace ksys::act::ai {

Query::Query(const InitArg& arg) : mActor(arg.actor), mDefIdx(arg.def_idx) {}

inline res::AIProgram* Query::getAIProg() const {
    return mActor->getParam()->getRes().mAIProgram;
}

bool Query::init(sead::Heap* heap) {
    if (mDefIdx != -1) {
        AIDefSet set;
        set.dynamic_params.num_params = 0;
        set.ai_tree_params.num_params = 0;

        auto* inst = AIClassDef::instance();
        const auto& query = getAIProg()->getQueries()[mDefIdx];
        inst->getDef(query.mClassName, &set, AIDefType::Query);

        if (mActor->getRootAi()->loadMapUnitParams(set.map_unit_params, heap) &&
            mActor->getRootAi()->loadAITreeParams(set.ai_tree_params, heap) &&
            (set.dynamic_params.num_params < 1 ||
             mParamPack.load(*mActor, set.dynamic_params, heap, AIDefInstParamKind::Dynamic))) {
            preInit_();
            return init_(heap);
        }
    }
    return false;
}

const char* Query::getName() const {
    if (mDefIdx == -1)
        return nullptr;

    return getAIProg()->getQueries()[mDefIdx].mClassName;
}

bool Query::getSInstParam(const f32** value, const sead::SafeString& param) const {
    const auto& def = getAIProg()->getQueries()[mDefIdx];
    return getAIProg()->getSInstParam(value, def, param);
}

bool Query::getDInstParam(sead::SafeString* value, const sead::SafeString& param) const {
    if (mParamPack.getString(value, param))
        return true;
    *value = "";
    return false;
}

bool Query::getDInstParam(const s32** value, const sead::SafeString& param) const {
    static s32 sDefault{};
    return getDInstParam_<s32>(value, param, AIDefParamType::Int, &sDefault);
}

bool Query::getDInstParam(const f32** value, const sead::SafeString& param) const {
    static f32 sDefault{};
    return getDInstParam_<f32>(value, param, AIDefParamType::Float, &sDefault);
}

bool Query::getDInstParam(const bool** value, const sead::SafeString& param) const {
    static bool sDefault{};
    auto ret = static_cast<bool*>(mParamPack.getAITreeVariablePointer(param, AIDefParamType::Bool));
    *value = ret ? ret : &sDefault;
    return ret != nullptr;
}

template class ClassContainer<Query>;

}  // namespace ksys::act::ai
