#pragma once

#include "dryadfacadeinternal.h"

Facade::Internal::Internal()
{

}

Facade::Internal::~Internal()
{
}

void Facade::Internal::RegisterMotif(const Motif& motif)
{
}

void Facade::Internal::RegisterLandscape(const LandscapeGraph& landscape)
{
}

void Facade::Internal::AddMotif(const unsigned motifId)
{
}

void Facade::Internal::RemoveMotif(const unsigned motifId)
{
}

void Facade::Internal::SetLandscape(const unsigned landscapeId)
{
}

void Facade::Internal::Generate(const unsigned durationToGenerate)
{
}

std::vector<NoteToPlay> Facade::Internal::Play(const unsigned durationToPlay)
{
	return {};
}