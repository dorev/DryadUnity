#include "core.h"

namespace Dryad
{

// Creates an empty score with an empty harmony graph
SharedPtr<Score> createScore()
{
    SharedPtr<Score> score = make<Score>();
    score->graph = make<HarmonyGraph>();
    score->graph->parentScore = score;

    return score;
}

// Used to set a group of node as potential progression starting point
void markAsEntry(InitializerList<SharedPtr<HarmonyNode>> nodes, bool value)
{
    for (SharedPtr<HarmonyNode> node : nodes)
    {
        node->isEntry = value;
    }
}

// Used to set a group of nodes as potential progression ending point
void markAsExit(InitializerList<SharedPtr<HarmonyNode>> nodes, bool value)
{
    for (SharedPtr<HarmonyNode> node : nodes)
    {
        node->isExit = value;
    }
}

// Adds other_node as edge if it is not already there
void addEdge(SharedPtr<HarmonyNode> node, SharedPtr<HarmonyNode> otherNode)
{
    for (const WeakPtr<HarmonyNode>& edge : node->edges)
    {
        SharedPtr<HarmonyNode> lockedEdge = edge.lock();

        if (lockedEdge == nullptr)
        {
            DEBUG_BREAK("Unable to add edge to node, it is expired");
        }

        if (lockedEdge == otherNode)
        {
            return;
        }
    }

    node->edges.push_back(otherNode);
}

// Adds multiple other_nodes as edges if they are not already there
void addMultipleEdges(SharedPtr<HarmonyNode> node, InitializerList<SharedPtr<HarmonyNode>> otherNodes)
{ 
    for (const SharedPtr<HarmonyNode>& other_node : otherNodes)
    {
        bool skip = false;

        for (const WeakPtr<HarmonyNode>& edge : node->edges)
        {
            SharedPtr<HarmonyNode> lockedEdge = edge.lock();

            if (lockedEdge == nullptr)
            {
                DEBUG_BREAK("Unable to add edge to node, it is expired");
            }

            if (lockedEdge == other_node)
            {
                skip = true;
                break;
            }
        }

        if (skip)
        {
            continue;
        }

        node->edges.push_back(other_node);
    }
}

// Used when generating progressions of a harmony_graph
// Checks if the node has not already been visit more often
// than allowed by the current graph setup
bool isVisitable(SharedPtr<HarmonyNode> node)
{
    return node->visitCount < node->maxVisit;
}

// Recursive function used to generate possible progression
// of a harmony_graph
void visit(SharedPtr<HarmonyNode> node, Vector<WeakPtr<HarmonyNode>>& progression)
{
    node->visitCount++;

    // Adds this node to the progression currently being built
    progression.push_back(node);

    // If the node can conclude a progression, store this progression in its
    // current state in the list of the harmony_graph
    if (node->isExit &&
        progression.size() > 1)
    {
        if (SharedPtr<HarmonyGraph> graph = node->parentHarmonyGraph.lock())
        {
            graph->progressions.push_back(progression);
        }
        else
        {
            DEBUG_BREAK("HarmonyGraph has been deallocated before its nodes");
        }
    }

    // For each harmonyNode connected to this one
    for (WeakPtr<HarmonyNode> edge : node->edges)
    {
        SharedPtr<HarmonyNode> lockedEdge = edge.lock();

        if (lockedEdge == nullptr)
        {
            DEBUG_BREAK("Node edge has expired, unable to explore graph");
        }

        // If the edge node has not been visited to often yet
        if (isVisitable(lockedEdge))
        {
            // Recursive call
            visit(lockedEdge, progression);
        }
    }

    // Every allowed progression from this point has been visited
    leave(node, progression);
}

// Takes a step back in the harmony_graph during progression generation
void leave(SharedPtr<HarmonyNode> node, Vector<WeakPtr<HarmonyNode>>& progression)
{
    if (node->visitCount == 0)
    {
        DEBUG_BREAK("harmonyNode is left more than it was visited");
    }

    // The harmonyNode is "un-visited" because it can still be encountered
    // on permutations originating from earlier nodes in the harmony_graph
    node->visitCount--;

    if (progression.empty())
    {
        DEBUG_BREAK("a progression cannot be empty if a node has been visited");
    }

    progression.pop_back();
}

// Triggers the exploration of the harmony_graph to generate all
// possible progressions bases on the contained harmony_nodes parameters
void generateAllProgressions(SharedPtr<HarmonyGraph> graph)
{
    Vector<WeakPtr<HarmonyNode>> progression;

    for (SharedPtr<HarmonyNode> node : graph->nodes)
    {
        if (node->isEntry)
        {
            visit(node, progression);
        }
    }
}

// Adds a harmonyNode to a harmony_graph
void addNode(SharedPtr<HarmonyGraph> graph, SharedPtr<HarmonyNode> node)
{
    node->parentHarmonyGraph = graph;
    graph->nodes.push_back(node);
}

// Adds multiple harmony_nodes to a harmony_graph
void addMultipleNodes(SharedPtr<HarmonyGraph> graph, InitializerList<SharedPtr<HarmonyNode>> nodes)
{
    for (SharedPtr<HarmonyNode> node : nodes)
    {
        node->parentHarmonyGraph = graph;
        graph->nodes.push_back(node);
    }
}

// Adds a degree to a scale if it does not exists yet in it
void addDegree(SharedPtr<Scale> scale, SharedPtr<Degree> addedDegree)
{
    for (const SharedPtr<Degree>& existingDegree : scale->degrees)
    {
        if (existingDegree == addedDegree)
        {
            return;
        }
    }

    scale->degrees.push_back(addedDegree);
}

// Adds multiple degrees to a scale if they don't appear in it yet
void addMultipleDegrees(SharedPtr<Scale> scale, InitializerList<SharedPtr<Degree>> addedDegrees)
{
    for (SharedPtr<Degree> degree : addedDegrees)
    {
        bool skip = false;

        for (const SharedPtr<Degree>& existingDegree : scale->degrees)
        {
            if (existingDegree == degree)
            {
                skip = true;
                break;
            }
        }

        if (skip)
        {
            continue;
        }

        scale->degrees.push_back(degree);
    }
}

// Creates a Major ionian scale
SharedPtr<Scale> createMajorScale()
{
    SharedPtr<Scale> majorScale = make<Scale>();

    majorScale->name = "Major Ionian scale";

    addMultipleDegrees(majorScale, 
    {
        make<Degree>(0, majorScale),
        make<Degree>(2, majorScale),
        make<Degree>(4, majorScale),
        make<Degree>(5, majorScale),
        make<Degree>(7, majorScale),
        make<Degree>(9, majorScale),
        make<Degree>(11, majorScale),
    });

    // Binds together the degrees so they can be used in cycles with
    // endless calls to next() and previous()
    for (uint i = 0; i < 7; ++i)
    {
        majorScale->degrees[i]->next = majorScale->degrees[(i + 1) % 7];
        majorScale->degrees[(i + 1) % 7]->previous = majorScale->degrees[i];
    }

    return majorScale;
}

// Evaluates the type of chord of a degree based on the following
// degrees in the scale
const Vector<uint>& getChordIntervals(SharedPtr<Degree> degree)
{
    // Do not execute this function if custom intervals are set
    if (degree->chordIntervals.size() > 0)
    {
        return degree->chordIntervals;
    }

    // Reach other degrees of the chord
    SharedPtr<Degree> third = next(next(degree));
    SharedPtr<Degree> fifth = next(next(third));
    SharedPtr<Degree> seventh = next(next(fifth));

    uint root = degree->intervalFromRoot;

    // Handle potential inverted order of inteval from root
    uint thirdInterval = third->intervalFromRoot < root
        ? third->intervalFromRoot + 12 - root
        : third->intervalFromRoot - root;

    // Validate that third is either major or minor
    if (thirdInterval != 3 && thirdInterval != 4)
    {
        DEBUG_BREAK("unsupported third chord interval");
    }

    // Handle potential inverted order of inteval from root
    uint fifthInterval = fifth->intervalFromRoot < root
        ? fifth->intervalFromRoot + 12 - root
        : fifth->intervalFromRoot - root;

    // Handle cases where the chord does not have a perfect fifth
    if (fifthInterval != 7)
    {
        if (fifthInterval == 6 && thirdInterval == 3)
        {
            degree->chordIntervals = Constants::ChordIntervals::Diminished;
            return degree->chordIntervals;
        }
        else if (fifthInterval == 8 && thirdInterval == 4)
        {
            degree->chordIntervals = Constants::ChordIntervals::Augmented;
            return degree->chordIntervals;
        }

        DEBUG_BREAK("unsupported fifth chord interval");
    }

    // Handle potential inverted order of inteval from root
    uint seventhInterval = seventh->intervalFromRoot < root
        ? seventh->intervalFromRoot + 12 - root
        : seventh->intervalFromRoot - root;

    // Validate that seventh is either major or minor
    if (seventhInterval != 10 && thirdInterval != 11)
    {
        DEBUG_BREAK("unsupported seventh chord interval");
    }

    // Identify complete chord interval following the previous validations
    if (thirdInterval == 3)
    {
        if (seventhInterval == 10)
        {
            degree->chordIntervals = Constants::ChordIntervals::m7;
        }
        else
        {
            degree->chordIntervals = Constants::ChordIntervals::mM7;
        }

        return degree->chordIntervals;
    }
    else
    {
        if (seventhInterval == 10)
        {
            degree->chordIntervals = Constants::ChordIntervals::M7;
        }
        else
        {
            degree->chordIntervals = Constants::ChordIntervals::MM7;
        }

        return degree->chordIntervals;
    }
}

// Create a harmony_graph based on the major ionian scale with a
// mostly agreeable configuration
SharedPtr<HarmonyGraph> createMajorGraph()
{
    SharedPtr<HarmonyGraph> graph = make<HarmonyGraph>();
    graph->scale = createMajorScale();

    for (uint i = 0; i < 7; ++i)
    {
        SharedPtr<HarmonyNode> node = make<HarmonyNode>();
        node->degree = graph->scale->degrees[i];
        node->parentHarmonyGraph = graph;
        addNode(graph, node);
    }

    SharedPtr<HarmonyNode> I = graph->nodes[0];
    SharedPtr<HarmonyNode> ii = graph->nodes[1];
    SharedPtr<HarmonyNode> iii = graph->nodes[2];
    SharedPtr<HarmonyNode> IV = graph->nodes[3];
    SharedPtr<HarmonyNode> V = graph->nodes[4];
    SharedPtr<HarmonyNode> vi = graph->nodes[5];
    SharedPtr<HarmonyNode> viid = graph->nodes[6];

    markAsEntry({I, iii, IV, V, vi});
    markAsExit({I, IV, V, vi, viid});

    addMultipleEdges(I,    {ii, iii, IV, V, vi, viid});
    addMultipleEdges(ii,   {V, viid});
    addMultipleEdges(iii,  {vi});
    addMultipleEdges(IV,   {V, viid});
    addMultipleEdges(V,    {I});
    addMultipleEdges(vi,   {ii, IV});
    addMultipleEdges(viid, {I, iii});

    I->maxVisit  = 2;
    IV->maxVisit = 2;
    V->maxVisit  = 2;
    vi->maxVisit = 2;

    return graph;
}

// Creates a motifVariation based on the motifConfig provided
// The energy is randomly distributed to the notes of the motif 
// and increases the pitch offset of the notes compared to their
// neighbours

// Should be called after spend_rhythmic_energy() otherwise there
// might be only one note in the motif
void spendMelodicEnergy(SharedPtr<MotifVariation> motif, SharedPtr<MotifConfig> motifConfig)
{
    if (motif->notes.size() == 0)
    {
        DEBUG_BREAK("no notes to spend melodic energy on");
    }

    Energy energyLeft = motifConfig->melodicEnergy;
    Energy max = motifConfig->maxMelodicEnergy;
    Energy min = motifConfig->minMelodicEnergy;

    // Created a vector to store "how the energy will be distributed"
    Vector<Energy> energyDistribution(motif->notes.size(), 0);

    while(energyLeft != 0)
    {
        // Randomly determines what note will get the energy
        uint targetedNote = static_cast<uint>(Random::range(0ULL, energyDistribution.size() - 1));
        Energy energyGiven = 0;

        // If the note can still receive energy, according to
        // the motifConfig
        if (energyDistribution[targetedNote] < max)
        {
            if (energyLeft >= min)
            {
                energyGiven = min == 0 ? 1 : min;
            }
            else
            {
                energyGiven = energyLeft;
            }
        }
        // Find another note to give energy too
        else
        {
            // Vector to store potential alternative energy receivers
            Vector<uint> candidateIndices;

            // Identify all notes still having room for more energy
            for (uint i = 0; i < motif->notes.size(); ++i)
            {
                if (energyDistribution[i] < max)
                {
                    candidateIndices.emplace_back(i);
                }
            }
            
            if (candidateIndices.empty())
            {
                // All notes are maxed on energy, unable to spend anymore energy
                break;
            }

            // Randomly select a note among the candidates
            targetedNote = Random::in(candidateIndices);

            if (energyLeft >= min)
            {
                energyGiven = min == 0 ? 1 : min;
            }
            else
            {
                energyGiven = energyLeft;
            }
        }

        // Prevent note from getting too much energy
        if (energyDistribution[targetedNote] + energyGiven > max)
        {
            energyGiven = max - energyDistribution[targetedNote];
        }

        // Update energy value of the note
        energyDistribution[targetedNote] += energyGiven;
        energyLeft -= energyGiven;
    }

    // Randomly decide for each note of the energy makes the note
    // get lower or higher and then assigned the final value
    // to the motif
    for (uint i = 0; i < energyDistribution.size(); ++i)
    {
        if (Random::fiftyFifty())
        {
            // Give a negative direction to energy
            energyDistribution[i] -= 2 * energyDistribution[i];
        }

        motif->notes[i]->offset = energyDistribution[i];
    }
}

// Breaks a motif duration into multiple smaller durations
void spendRythmicEnergy(SharedPtr<MotifVariation> motif, SharedPtr<MotifConfig> motifConfig)
{
    uint duration = motifConfig->duration;
    uint energyLeft = motifConfig->rhythmicEnergy;

    Vector<uint> notesDurations({ duration });
    Vector<std::pair<uint, uint>> solutions;

    while(energyLeft--)
    {
        uint targetedNote = Random::range(0ULL, notesDurations.size() - 1);
        uint notesDuration = notesDurations[targetedNote];

        if (notesDuration > Constants::Duration::Sixteenth)
        {
            getEquivalentDurationPairs(notesDuration, solutions);
            const std::pair<uint, uint>& selectedSolution = Random::in(solutions);
            notesDurations[targetedNote] = selectedSolution.first;
            notesDurations.insert(notesDurations.begin() + targetedNote, selectedSolution.second);
        }
        else if (notesDuration == Constants::Duration::Sixteenth)
        {
            // Find another note to split
            Vector<uint> candidateIndices;

            for (uint i = 0; i < (uint)notesDurations.size(); ++i)
            {
                if (notesDurations[i] > Constants::Duration::Sixteenth)
                {
                    candidateIndices.emplace_back(i);
                }
            }

            if (candidateIndices.empty())
            {
                // notesDurations vector contains only sixteenth,
                // unable to spend anymore energy
                break;
            }

            uint new_targeted_note = Random::in(candidateIndices);
            getEquivalentDurationPairs(notesDurations[new_targeted_note], solutions);
            const std::pair<uint, uint>& selectedSolution = Random::in(solutions);
            notesDurations[new_targeted_note] = selectedSolution.first;
            notesDurations.insert(notesDurations.begin() + new_targeted_note, selectedSolution.second);
        }
        else
        {
            DEBUG_BREAK("reached unsupported rhythmic fraction during rhythmic energy spending");
        }
    }

    for(uint i = 0; i < notesDurations.size(); ++i)
    {
        SharedPtr<Note> new_note = make<Note>();
        new_note->duration = duration;
        new_note->motif = motif;
        motif->notes.emplace_back(new_note);
    }
}

void generateMotif(SharedPtr<MotifVariation> motif, SharedPtr<MotifConfig> motifConfig)
{
    if (motifConfig->duration % Constants::Duration::Quarter)
    {
        DEBUG_BREAK("motif duration should be a factor of quarter notes");
    }
    if (motifConfig->duration > 2 * Constants::Duration::Whole)
    {
        DEBUG_BREAK("requested motif duration is too long");
    }

    spendRythmicEnergy(motif, motifConfig);
    spendMelodicEnergy(motif, motifConfig);
}

void generateMotif(SharedPtr<Motif> motif, SharedPtr<MotifConfig> motifConfig)
{
    if (motifConfig->duration % Constants::Duration::Quarter)
    {
        DEBUG_BREAK("motif duration should be a factor of quarter notes");
    }
    if (motifConfig->duration > 2 * Constants::Duration::Whole)
    {
        DEBUG_BREAK("requested motif duration is too long");
    }

    if (motif->variations.size() == 0)
    {
        motif->variations.emplace_back(make<MotifVariation>());
        generateMotif(motif->variations[0], motifConfig);
        return;
    }

    DEBUG_BREAK("generate_motif should not be called on a motif already containing variations");
}

void applyProgression(SharedPtr<Phrase> phrase, const Vector<WeakPtr<HarmonyNode>>& progression, FittingMode fittingMode)
{
    Vector<SharedPtr<Measure>>& measures = phrase->measures;
    uint progressionSize = progression.size();
    uint phraseSize = measures.size();

    if (!isPowerOf2(phraseSize))
    {
        DEBUG_BREAK("A phrase should be a power of 2");
    }

    // Perfect fit! Easy dispatch
    if (progressionSize == phraseSize)
    {
        for (uint i = 0; i < phraseSize; ++i)
        {
            measures[i]->progression.emplace_back(progression[i]);
        }
        return;
    }

    Vector<uint> degreesDistribution(phraseSize, 0);

    // Values that cannot be initialized in the switch case
    uint chordsToFit = progressionSize;
    uint offset = phraseSize;
    uint measure = 0;

    switch (fittingMode)
    {
    case FittingMode::PowerOf2Left:
    case FittingMode::PowerOf2Right:

        for (uint n = 0; n < chordsToFit; ++n)
        {
            offset = phraseSize;
            measure = 0;

            for(uint bit = 0; bit < std::log2(phraseSize); ++bit)
            {
                offset >>= 1;

                bool bit_of_n_is_off = !((1ULL << bit) & n);

                if (bit_of_n_is_off)
                {
                    measure += offset;
                }
            }

            if (progressionSize < phraseSize ||
                fittingMode == FittingMode::PowerOf2Left)
            {
                ++degreesDistribution[(phraseSize - 1) - measure];
            }
            else if (fittingMode == FittingMode::PowerOf2Right)
            {
                ++degreesDistribution[measure];
            }
            else
            {
                DEBUG_BREAK("We should not reach this point");
            }
        }

        break;

    case FittingMode::CompactLeft:
    case FittingMode::CompactRight:

        while (chordsToFit != 0)
        {
            bool break_while = false;

            for (measure = phraseSize; measure > (phraseSize - offset); --measure)
            {
                if (progressionSize < phraseSize ||
                    fittingMode == FittingMode::CompactLeft)
                {
                    ++degreesDistribution[phraseSize - measure];
                }
                else if (fittingMode == FittingMode::CompactRight)
                {
                    ++degreesDistribution[measure - 1];
                }
                else
                {
                    DEBUG_BREAK("We should not reach this point");
                }

                if (--chordsToFit == 0)
                {
                    break_while = true;
                    break;
                }
            }
            
            if (break_while)
            {
                break;
            }

            offset >>= 1;

            if (offset == 0)
            {
                offset = phraseSize;
            }
        }

        break;

    default:
        break;
    }

    uint progressionIndex = 0;

    for(uint i = 0; i < phraseSize; ++i)
    {
        // For the number of chords in that measure
        while (degreesDistribution[i]--)
        {
            measures[i]->progression.emplace_back(progression[progressionIndex++]);
        }
    }
}

void applyMotif(SharedPtr<Phrase> phrase, SharedPtr<MotifVariation> motifVariation, SharedPtr<Voice> voice)
{
    // Append the motif in loop until the phrase is fully filled
    for (;;)
    {
        // For each note of the motif
        for (SharedPtr<Note> note : motifVariation->notes)
        {
            // Find the last measure with room for the target voice
            for (SharedPtr<Measure> measure : phrase->measures)
            {
                uint voiceDuration = getTotalVoiceDuration(measure, voice);
                
                if (voiceDuration < measure->duration)
                {
                    SharedPtr<Note> clonedNote = clone(note);
                    clonedNote->voice = voice;
                    appendNote(measure, clonedNote);

                    // Append next note
                    break;
                }
                else if (measure == last(phrase->measures))
                {
                    // Whole phrase has been covered
                    return;
                }
            }
        }
    }
}

void appendPhrase(SharedPtr<Score> score, SharedPtr<Phrase> phrase)
{
    Vector<SharedPtr<Phrase>>& phrases = score->phrases;
    phrase->parentScore = score;

    if (!phrases.empty())
    {
        if (SharedPtr<Phrase> lastPhrase = last(phrases))
        {
            lastPhrase->next = phrase;
            phrase->previous = lastPhrase;
        }
    }

    phrases.emplace_back(phrase);
}

void appendMeasure(SharedPtr<Phrase> phrase, SharedPtr<Measure> measure)
{
    Vector<SharedPtr<Measure>>& measures = phrase->measures;
    measure->parentPhrase = phrase;

    if (measures.empty())
    {
        if (SharedPtr<Measure> lastMeasure = last(measures))
        {
            lastMeasure->next = measure;
            measure->previous = lastMeasure;
        }
        else if (SharedPtr<Phrase> previousPhrase = previous(phrase))
        {
            SharedPtr<Measure> lastMeasure = last(previousPhrase->measures);
            lastMeasure->next = measure;
            measure->previous = lastMeasure;
        }
    }
    else
    {
        if (SharedPtr<Measure> lastMeasure = last(measures))
        {
            lastMeasure->next = measure;
            measure->previous = lastMeasure;
        }
    }

    if (measure->positions.size() != 0)
    {
        if (SharedPtr<Measure> lastMeasure = last(measures))
        {
            if (SharedPtr<Position> lastPosition = last(lastMeasure->positions))
            {
                SharedPtr<Position> position = first(measure->positions);
                position->previous = lastPosition;
                lastPosition->next = position;
            }
        }
    }

    measures.emplace_back(measure);
}

void appendNote(SharedPtr<Measure> measure, SharedPtr<Note> note)
{
    uint voiceDuration = getTotalVoiceDuration(measure, note->voice);

    if (voiceDuration >= measure->duration)
    {
        // Try to append on next measure
        if (SharedPtr<Measure> nextMeasure = next(measure))
        {
            appendNote(nextMeasure, note);
        }

        return;
    }

    SharedPtr<Position> notePosition = getPositionAtTime(measure, voiceDuration);

    if (notePosition == nullptr)
    {
        notePosition = insertPositionAtTime(measure, voiceDuration);
    }

    if( (note->duration + notePosition->measureTime) > measure->duration)
    {
        // Overflow note duration to next measure
        // TODO: liaisons?
        uint durationCurrentMeasure = measure->duration - notePosition->measureTime;

        // Try to append on next measure
        if (SharedPtr<Measure> nextMeasure = next(measure))
        {
            uint durationNextMeasure = note->duration - durationCurrentMeasure;
            SharedPtr<Note> nextMeasureClonedNote = clone(note);
            nextMeasureClonedNote->duration = durationNextMeasure;
            appendNote(nextMeasure, nextMeasureClonedNote);
        }

        note->duration = durationCurrentMeasure;
    }

    note->parentPosition = notePosition;
    notePosition->notes.emplace_back(note);
}

SharedPtr<Position> getPositionAtTime(SharedPtr<Measure> measure, uint time)
{
    for (SharedPtr<Position> position : measure->positions)
    {
        if (position->measureTime == time)
        {
            return position;
        }
    }

    return nullptr;
}

SharedPtr<Position> insertPositionAtTime(SharedPtr<Measure> measure, uint time)
{
    SharedPtr<Position> positionBefore = nullptr;
    SharedPtr<Position> positionAfter = nullptr;
    Vector<SharedPtr<Position>>& positions = measure->positions;

    for (SharedPtr<Position> position : positions)
    {
        uint positionTime = position->measureTime;

        if (positionTime == time)
        {
            return position;
        }
        else if (positionTime > time)
        {
            positionAfter = position;
            break;
        }

        positionBefore = position;
    }

    if (positionBefore == nullptr)
    {
        SharedPtr<Position> newPosition = make<Position>(measure, time);
        newPosition->parentMeasure = measure;

        if (time != 0)
        {
            // Add initial position with a rest
            SharedPtr<Position> zeroPosition = make<Position>(measure);
            zeroPosition->parentMeasure = measure;

            getHarmonyNode(zeroPosition);
            newPosition->previous = zeroPosition;
            zeroPosition->next = newPosition;

            if (SharedPtr<Measure> previousMeasure = previous(measure))
            {
                if (SharedPtr<Position> lastPosition = last(previousMeasure->positions))
                {
                    zeroPosition->previous = lastPosition;
                }
            }

            positions.emplace_back(zeroPosition);
        }

        positions.emplace_back(newPosition);
        getHarmonyNode(newPosition);
        return newPosition;
    }

    for(auto it = positions.begin(); it != positions.end(); ++it)
    {
        if (*it == positionBefore)
        {
            SharedPtr<Position> newPosition = make<Position>();
            newPosition->parentMeasure = measure;

            newPosition->measureTime = time;
            newPosition->parentMeasure = measure;
            positions.insert(it, newPosition);

            positionBefore->next = newPosition;
            newPosition->previous = positionBefore;

            if (positionAfter != nullptr)
            {
                newPosition->next = positionAfter;
                positionAfter->previous = newPosition;
            }

            getHarmonyNode(newPosition);
            return newPosition;
        }
    }

    DEBUG_BREAK("this section should never be reached");
}

uint getTotalVoiceDuration(SharedPtr<Measure> measure, SharedPtr<Voice> voice)
{
    if (measure->positions.size() == 0)
    {
        return 0;
    }

    bool voiceExistsInMeasure = false;

    // Inspect first position of measure to check if the targeted voice is
    // present in the measure
    for (SharedPtr<Note> note : measure->positions[0]->notes)
    {
        if (note->voice == voice)
        {
            voiceExistsInMeasure = true;
            break;
        }
    }

    if (!voiceExistsInMeasure)
    {
        return 0;
    }

    uint total_duration = 0;

    for (SharedPtr<Position> position : measure->positions)
    {
        for (SharedPtr<Note> note : position->notes)
        {
            if (note->voice == voice)
            {
                total_duration += note->duration;

                // It is assumed that there can only be
                // one note of a voice in a position
                break;
            }
        }
    }

    return total_duration;
}

void relinkPhrase(SharedPtr<Phrase> phrase)
{
    Vector<SharedPtr<Measure>>& measures = phrase->measures;
    uint measureCount = (uint)measures.size();

    for (uint measureIndex = 0; measureIndex < measureCount; ++measureIndex)
    {
        SharedPtr<Measure> measure = measures[measureIndex];
        measure->parentPhrase = phrase;

        if (measureIndex == 0)
        {
            if (SharedPtr<Phrase> previousPhrase = previous(phrase))
            {
                if (SharedPtr<Measure> lastMeasure = last(previousPhrase->measures))
                {
                    measure->previous = lastMeasure;
                    lastMeasure->next = measure;
                }
            }
        }
        else
        {
            measure->previous = measures[measureIndex - 1];
        }

        if (measureIndex < (measureCount - 1))
        {
            measure->next = measures[measureIndex + 1];
        }
        else
        {
            measure->next.reset();
        }

        Vector<SharedPtr<Position>>& positions = measure->positions;
        uint positionCount = (uint)positions.size();

        for (uint positionIndex = 0; positionIndex < positionCount; ++positionIndex)
        {
            SharedPtr<Position> position = positions[positionIndex];
            position->parentMeasure = measure;

            if (positionIndex == 0)
            {
                if (SharedPtr<Measure> previousMeasure = previous(measure))
                {
                    if (SharedPtr<Position> previousPosition = last(previousMeasure->positions))
                    {
                        position->previous = previousPosition;
                        previousPosition->next = position;
                    }
                }
            }
            else
            {
                position->previous = positions[positionIndex - 1];
            }

            if (positionIndex < (positionCount - 1))
            {
                position->next = positions[positionIndex + 1];
            }
            else
            {
                position->next.reset();
            }
        }
    }
}

void relinkScore(SharedPtr<Score> score)
{
    Vector<SharedPtr<Phrase>>& phrases = score->phrases;
    uint phraseCount = (uint)phrases.size();

    for (uint phraseIndex = 0; phraseIndex < phraseCount; ++phraseIndex)
    {
        SharedPtr<Phrase> phrase = phrases[phraseIndex];
        phrase->parentScore = score;

        if (phraseIndex != 0)
        {
            phrase->previous = phrases[phraseIndex - 1];
        }

        if (phraseIndex < (phraseCount - 1))
        {
            phrase->next = phrases[phraseIndex + 1];
        }
        else
        {
            phrase->next.reset();
        }

        relinkPhrase(phrase);
    }
}

void applyScale(SharedPtr<Note> note, SharedPtr<Scale> scale, SharedPtr<ScaleConfig> scaleConfig)
{
    SharedPtr<Position> position = note->parentPosition.lock();

    if (position == nullptr)
    {
        DEBUG_BREAK("note has no parent_position assigned");
    }

    SharedPtr<HarmonyNode> harmonyNode = position->harmonyNode;

    SharedPtr<Degree> harmonyNodeDegree = harmonyNode->degree.lock();

    if (harmonyNodeDegree == nullptr)
    {
        DEBUG_BREAK("No degree associated with node");
    }

    uint midiValue = note->midi = scaleConfig->root +
        harmonyNodeDegree->intervalFromRoot +
        harmonyNode->alteration +
        harmonyNode->modulation +
        (12 * note->voice->octave);

    note->octave = midiValue / 12;

    uint absoluteNote = midiValue % 12;

    if (contains(absoluteNote, Constants::BaseNotes))
    {
        note->name = Constants::NoteNamesSharp[absoluteNote];
        note->accidental = Accidental::None;
    }
    else
    {
        switch (scaleConfig->accidental)
        {
            default:
            case Accidental::None:
            case Accidental::Sharp:
                note->accidental = Accidental::Sharp;
                note->name = Constants::NoteNamesSharp[absoluteNote];
                note->step = Constants::NoteNamesSharp[absoluteNote - 1];
                return;

            case Accidental::Flat:
                note->accidental = Accidental::Flat;
                note->name = Constants::NoteNamesFlat[absoluteNote];
                note->step = Constants::NoteNamesFlat[absoluteNote + 1];
                return;
        }
    }
}

// Score construction
void applyScale(SharedPtr<Score> score, SharedPtr<Scale> scale, SharedPtr<ScaleConfig> scaleConfig)
{
    relinkScore(score);

    for (SharedPtr<Phrase> phrase : score->phrases)
    {
        for (SharedPtr<Measure> measure : phrase->measures)
        {
            for (SharedPtr<Position> position : measure->positions)
            {
                for (SharedPtr<Note> note : position->notes)
                {
                    applyScale(note, scale, scaleConfig);
                }
            }
        }
    }
}

// Walk up previous measures and returns the first node found
SharedPtr<HarmonyNode> findLatestHarmonyNode(SharedPtr<Position> position)
{
    SharedPtr<Measure> measure = position->parentMeasure.lock();

    if (measure == nullptr)
    {
        DEBUG_BREAK("a position is always expected to have a parent_measure");
    }

    for (;;)
    {
        measure = previous(measure);

        if (measure == nullptr)
        {
            return nullptr;
        }

        if(!measure->progression.empty())
        {
            return last(measure->progression);
        }
    }
}

// Find the HarmonyNode influencing this position
SharedPtr<HarmonyNode> getHarmonyNode(SharedPtr<Position> position)
{
    SharedPtr<Measure> measure = position->parentMeasure.lock();

    if (measure == nullptr)
    {
        position->harmonyNode = nullptr;
        DEBUG_BREAK("a position is always expected to have a parent_measure");
    }

    uint progressionSize = (uint)measure->progression.size();

    if (progressionSize == 0)
    {
        return position->harmonyNode = findLatestHarmonyNode(position);
    }
    else if (progressionSize == 1)
    {
        return position->harmonyNode = measure->progression[0];
    }

    uint chord_duration = measure->duration / progressionSize;
    uint good_node_index = position->measureTime / chord_duration;

    return position->harmonyNode = measure->progression[good_node_index];
}

// Score rendering
void renderMusicXml(Score* score)
{
}

} // namespace Dryad