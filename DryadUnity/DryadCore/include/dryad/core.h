#pragma once

#include "types.h"

#include "degree.h"
#include "harmonygraph.h"
#include "harmonynode.h"
#include "measure.h"
#include "motif.h"
#include "motifconfig.h"
#include "motifvariation.h"
#include "constants.h"
#include "definitions.h"
#include "note.h"
#include "phrase.h"
#include "position.h"
#include "utils.h"
#include "scale.h"
#include "scaleconfig.h"
#include "score.h"
#include "utils.h"
#include "voice.h"

namespace Dryad
{

// Harmony node
void markAsEntry(InitializerList<SharedPtr<HarmonyNode>> nodes, bool value = true);
void markAsExit(InitializerList<SharedPtr<HarmonyNode>> nodes, bool value = true);
void addEdge(SharedPtr<HarmonyNode> node, SharedPtr<HarmonyNode> other_node);
void addMultipleEdges(SharedPtr<HarmonyNode> node, InitializerList<SharedPtr<HarmonyNode>> other_nodes);
bool isVisitable(SharedPtr<HarmonyNode> node);
void visit(SharedPtr<HarmonyNode> node, Vector<WeakPtr<HarmonyNode>>& progression);
void leave(SharedPtr<HarmonyNode> node, Vector<WeakPtr<HarmonyNode>>& progression);

// Harmony graph
SharedPtr<HarmonyGraph> createMajorGraph();
void addNode(SharedPtr<HarmonyGraph> graph, SharedPtr<HarmonyNode> node);
void addMultipleNodes(SharedPtr<HarmonyGraph> graph, InitializerList<SharedPtr<HarmonyNode>> nodes);
void generateAllProgressions(SharedPtr<HarmonyGraph> graph);

// Scale
SharedPtr<Scale> createMajorScale();
void addDegree(SharedPtr<Scale> scale, SharedPtr<Degree> degree);
void addMultipleDegrees(SharedPtr<Scale> scale, InitializerList<SharedPtr<Degree>> degrees);
const Vector<uint>& getChordIntervals(SharedPtr<Degree> degree);


// Motif
void spendMelodicEnergy(SharedPtr<MotifVariation> motif, SharedPtr<MotifConfig> motifConfig);
void spendRythmicEnergy(SharedPtr<MotifVariation> motif, SharedPtr<MotifConfig> motifConfig);
void generateMotif(SharedPtr<MotifVariation> motif, SharedPtr<MotifConfig> motifConfig);
void generateMotif(SharedPtr<Motif> motif, SharedPtr<MotifConfig> motifConfig);

// Phrase
void applyProgression(SharedPtr<Phrase> phrase, const Vector<WeakPtr<HarmonyNode>>& progression, FittingMode fittingMode = FittingMode::PowerOf2Right);
void applyMotif(SharedPtr<Phrase> phrase, SharedPtr<MotifVariation> motifVariation, SharedPtr<Voice> voice);
void appendPhrase(SharedPtr<Score> score, SharedPtr<Phrase> phrase);
void appendMeasure(SharedPtr<Phrase> phrase, SharedPtr<Measure> measure);
void appendNote(SharedPtr<Measure> measure, SharedPtr<Note> note);

// Measure
uint getTotalVoiceDuration(SharedPtr<Measure> measure, SharedPtr<Voice> voice);
SharedPtr<Position> getPositionAtTime(SharedPtr<Measure> measure, uint time);
SharedPtr<Position> insertPositionAtTime(SharedPtr<Measure> measure, uint time);

// Score construction
SharedPtr<Score> createScore();
void relinkScore(SharedPtr<Score> score);
void relinkPhrase(SharedPtr<Phrase> phrase);
void applyScale(SharedPtr<Score> score, SharedPtr<Scale> scale, SharedPtr<ScaleConfig> scaleConfig);
SharedPtr<HarmonyNode> getHarmonyNode(SharedPtr<Position> position);

// Score rendering
void renderMusicXml(Score* score);

} // namespace Dryad