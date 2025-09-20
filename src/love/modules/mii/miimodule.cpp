#include "miimodule.hpp"

#include <sol/sol.hpp>
#include "mii.h"
#include "classes/miisafe.hpp"

namespace {
    std::vector<std::shared_ptr<love::mii::MiiSafe>> miis;
}

namespace love {
    namespace mii {
        void __init(sol::state &luastate) {
            Mii* loadedMiis = loadMiis_Wii();
            if (!loadedMiis) return;

            miis.clear();
            for (int i = 0; i < 100; ++i) {
                if (loadedMiis[i].invalid) break;
                miis.push_back(std::make_shared<love::mii::MiiSafe>(loadedMiis[i]));
            }

            __registerTypes(luastate);
        }

        void __registerTypes(sol::state &luastate) {
            luastate.new_usertype<love::mii::MiiSafe>(
                "Mii",
                sol::no_constructor,

                "name", sol::property(&love::mii::MiiSafe::nameStr),
                "female", &Mii::female,
                "height", &Mii::height,
                "weight", &Mii::weight,

                "miiID1", &Mii::miiID1,
                "miiID2", &Mii::miiID2,
                "miiID3", &Mii::miiID3,
                "miiID4", &Mii::miiID4,
                "systemID0", &Mii::systemID0,
                "systemID1", &Mii::systemID1,
                "systemID2", &Mii::systemID2,
                "systemID3", &Mii::systemID3,

                "faceShape", &Mii::faceShape,
                "skinColor", &Mii::skinColor,
                "facialFeature", &Mii::facialFeature,
                "downloaded", &Mii::downloaded,

                "hairType", &Mii::hairType,
                "hairColor", &Mii::hairColor,
                "hairPart", &Mii::hairPart,

                "eyebrowType", &Mii::eyebrowType,
                "eyebrowRotation", &Mii::eyebrowRotation,
                "eyebrowColor", &Mii::eyebrowColor,
                "eyebrowSize", &Mii::eyebrowSize,
                "eyebrowVertPos", &Mii::eyebrowVertPos,
                "eyebrowHorizSpacing", &Mii::eyebrowHorizSpacing,

                "eyeType", &Mii::eyeType,
                "eyeRotation", &Mii::eyeRotation,
                "eyeVertPos", &Mii::eyeVertPos,
                "eyeColor", &Mii::eyeColor,
                "eyeSize", &Mii::eyeSize,
                "eyeHorizSpacing", &Mii::eyeHorizSpacing,

                "noseType", &Mii::noseType,
                "noseSize", &Mii::noseSize,
                "noseVertPos", &Mii::noseVertPos,

                "lipType", &Mii::lipType,
                "lipColor", &Mii::lipColor,
                "lipSize", &Mii::lipSize,
                "lipVertPos", &Mii::lipVertPos,

                "glassesType", &Mii::glassesType,
                "glassesColor", &Mii::glassesColor,
                "glassesSize", &Mii::glassesSize,
                "glassesVertPos", &Mii::glassesVertPos,

                "mustacheType", &Mii::mustacheType,
                "beardType", &Mii::beardType,
                "facialHairColor", &Mii::facialHairColor,
                "mustacheSize", &Mii::mustacheSize,
                "mustacheVertPos", &Mii::mustacheVertPos,

                "mole", &Mii::mole,
                "moleSize", &Mii::moleSize,
                "moleVertPos", &Mii::moleVertPos,
                "moleHorizPos", &Mii::moleHorizPos,
                
                "creator", sol::property(&love::mii::MiiSafe::creatorStr)
            );
        }

        sol::object getMiis(sol::state &luastate) {
            sol::table t = luastate.create_table();
            for (size_t i = 0; i < miis.size(); ++i) {
                t[i + 1] = miis[i];
            }
            return sol::make_object(luastate, t);
        }
    }
}