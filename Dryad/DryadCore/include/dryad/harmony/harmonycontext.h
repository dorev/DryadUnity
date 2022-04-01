#pragma once

#include "dryad/definitions.h"
#include "dryad/error.h"
#include "dryad/result.h"
#include "dryad/types.h"

#include "dryad/harmony/landscapegraph.h"
#include "dryad/harmony/motif.h"
#include "dryad/harmony/harmonizer.h"

namespace Dryad
{

class Session;
class Score;

struct MotifContext
{
    MotifContext(const MotifDescriptor& motifDescriptor)
        : motif(motifDescriptor)
        , level(0)
    {
    }

    Motif motif;
    U32 level;
};

/* DEPRECATED?
struct HarmonyContextSnapshot
{
    LandscapeGraph* currentLandscape;
    LandscapeGraph* previousLandscape;
    Scale* scale;
    Note scaleRoot;
    Map<Motif*, U32> motifLevels;

    bool HasNoMotifs() const
    {
        for (const auto& [motif, level] : motifLevels)
        {
            if (level > 0)
                return false;
        }
        return true;
    }

    bool operator==(const HarmonyContextSnapshot& other) const
    {
        return currentLandscape == other.currentLandscape
            && previousLandscape == other.previousLandscape
            && scale == other.scale
            && scaleRoot == other.scaleRoot
            && motifLevels == other.motifLevels;
    }
};
*/

class HarmonyContext
{
public:
    HarmonyContext()
        : _currentLandscape(nullptr)
        , _previousLandscape(nullptr)
        , _landscapeChanged(false)
        , _currentScale(nullptr)
        , _currentScaleRoot(0)
    {
    }

    Result<> SetLandscape(const String& landscapeName)
    {
        LandscapeGraph* findResult = FindLandscape(landscapeName);
        if (findResult == nullptr)
            return { ErrorCode::LandscapeDoesNotExist };
        _landscapeChanged = true;
        _previousLandscape = _currentLandscape;
        _currentLandscape = findResult;
        return Success;
    }

    Result<> SetMotifCount(const String& motifName, S32 amount)
    {
        if(amount < 0)
            return { ErrorCode::UnexpectedNegativeNumber };
        MotifContext* motifContext = FindMotifContext(motifName);
        if (motifContext == nullptr)
            return { ErrorCode::MotifDoesNotExist };
        S32 motifCount = static_cast<S32>(motifContext->level);
        S32 delta =  motifCount - amount;
        if (delta != 0)
        {
            if ((motifCount + delta) < 0)
                motifContext->level = 0;
            else
                motifContext->level += delta;
            _motifsChanged = true;
        }
        return Success;
    }

    Result<> IncrementMotifCount(const String& motifName, S32 increment)
    {
        MotifContext* motifContext = FindMotifContext(motifName);
        if (motifContext == nullptr)
            return { ErrorCode::MotifDoesNotExist };
        if (increment != 0)
        {
            if ((motifContext->level + increment) < 0)
                motifContext->level = 0;
            else
                motifContext->level += increment;
            _motifsChanged = true;
        }
        return Success;
    }

    Result<Motif*> RegisterMotif(const MotifDescriptor& descriptor)
    {
        if (FindMotif(descriptor.GetName()) != nullptr)
            return { ErrorCode::MotifAlreadyExists };
        auto emplaceResult = _motifContexts.emplace(descriptor.GetName(), descriptor);
        bool motifInserted = emplaceResult.second;
        if (motifInserted)
        {
            Motif* motifPointer = &(emplaceResult.first->second.motif);
            _motifContexts.at(motifPointer->GetName()).level = 0;
            return motifPointer;
        }
        return { ErrorCode::UnableToStoreData };
    }

    Result<LandscapeGraph*> RegisterLandscape(const LandscapeGraphDescriptor& descriptor)
    {
        if (FindLandscape(descriptor.GetName()) != nullptr)
            return { ErrorCode::MotifAlreadyExists };
        auto emplaceResult = _landscapes.emplace(descriptor.GetName(), descriptor);
        bool landscapeInserted = emplaceResult.second;
        if (landscapeInserted)
        {
            LandscapeGraph* landscapePointer = &(emplaceResult.first->second);
            return landscapePointer;
        }
        return { ErrorCode::UnableToStoreData };
    }

    bool LandscapeChanged()
    {
        if (_landscapeChanged)
        {
            _landscapeChanged = false;
            return true;
        }
        return false;
    }

    bool MotifsChanged()
    {
        if (_motifsChanged)
        {
            _motifsChanged = false;
            return true;
        }
        return false;
    }

    bool ScaleChanged()
    {
        if (_scaleChanged)
        {
            _scaleChanged = false;
            return true;
        }
        return false;
    }

    bool HasChanged()
    {
        return MotifsChanged() || LandscapeChanged() || ScaleChanged();
    }

    Motif* FindMotif(const String& motifName)
    {
        auto findResult = _motifContexts.find(motifName);
        if (findResult == _motifContexts.end())
            return nullptr;
        return &(findResult->second.motif);
    }

    MotifContext* FindMotifContext(const String& motifName)
    {
        auto findResult = _motifContexts.find(motifName);
        if (findResult == _motifContexts.end())
            return nullptr;
        return &(findResult->second);
    }

    bool HasNoMotifs() const
    {
        for (const auto& [name, motifContext] : _motifContexts)
        {
            if (motifContext.level > 0)
                return false;
        }
        return true;
    }

    LandscapeGraph* FindLandscape(const String& landscapeName)
    {
        auto findResult = _landscapes.find(landscapeName);
        if (findResult == _landscapes.end())
            return nullptr;
        return &(findResult->second);
    }

private:
    bool _landscapeChanged;
    LandscapeGraph* _currentLandscape;
    LandscapeGraph* _previousLandscape;
    Map<String, LandscapeGraph> _landscapes;
    
    bool _motifsChanged;
    Map<String, MotifContext> _motifContexts;

    bool _scaleChanged;
    Scale* _currentScale;
    Note _currentScaleRoot;
    Map<String, Scale> _scales;
};

} // namespace Dryad