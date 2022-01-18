#include "dryadfacade.h"
#include "dryadfacadeinternal.h"

Facade::Facade()
	: _internal(std::make_unique<Internal>())
{
}

Facade::~Facade()
{
}

void Facade::RegisterMotif(const Motif& motif)
{
	_internal->RegisterMotif(motif);
}

void Facade::RegisterLandscape(const LandscapeGraph& landscape)
{
	_internal->RegisterLandscape(landscape);
}

void Facade::AddMotif(const unsigned motifId)
{
	_internal->AddMotif(motifId);
}

void Facade::RemoveMotif(const unsigned motifId)
{
	_internal->RemoveMotif(motifId);
}

void Facade::SetLandscape(const unsigned landscapeId)
{
	_internal->SetLandscape(landscapeId);
}

void Facade::Generate(const unsigned durationToGenerate)
{
	_internal->Generate(durationToGenerate);
}

std::vector<NoteToPlay> Facade::Play(const unsigned durationToPlay)
{
	return _internal->Play(durationToPlay);
}
