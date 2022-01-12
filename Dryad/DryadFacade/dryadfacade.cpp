#include "dryadfacade.h"
#include "dryadfacadeinternal.h"

Facade::Facade()
	: _facadeInternal(std::make_unique<Internal>())
{
}

Facade::~Facade()
{
}

void Facade::RegisterMotif(Motif motif)
{
}

void Facade::RegisterLandscape(LandscapeGraph landscape)
{
}

void Facade::AddMotif(unsigned motifId)
{
}

void Facade::RemoveMotif(unsigned motifId)
{
}

void Facade::SetLandscape(unsigned landscapeId)
{
}

void Facade::Generate(unsigned durationToGenerate)
{
}

std::vector<NoteToPlay> Facade::Play(unsigned durationToPlay)
{
	return {};
}
