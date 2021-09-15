#include "gtest/gtest.h"
#include "dryad/core.h"

namespace Dryad
{

class DryadTests : public ::testing::Test
{
protected:

    DryadTests()
    {
    }

    ~DryadTests() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

#define EXPECT_NO_LEAK_FROM_PREVIOUS_TEST           \
    EXPECT_EQ(0ULL, Degree::GetCount());            \
    EXPECT_EQ(0ULL, HarmonyGraph::GetCount());      \
    EXPECT_EQ(0ULL, HarmonyNode::GetCount());       \
    EXPECT_EQ(0ULL, Measure::GetCount());           \
    EXPECT_EQ(0ULL, Motif::GetCount());             \
    EXPECT_EQ(0ULL, MotifVariation::GetCount());    \
    EXPECT_EQ(0ULL, Position::GetCount());          \
    EXPECT_EQ(0ULL, Note::GetCount());              \
    EXPECT_EQ(0ULL, Phrase::GetCount());            \
    EXPECT_EQ(0ULL, Scale::GetCount());             \
    EXPECT_EQ(0ULL, Voice::GetCount());             \

TEST_F(DryadTests, ApplyProgressionToPhrase)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    SharedPtr<HarmonyGraph> graph = createMajorGraph();
    generateAllProgressions(graph);

    uint epoch = 100;

    for (uint n = 0; n < epoch; ++n)
    {
        EXPECT_EQ(0ULL, Phrase::GetCount());
        EXPECT_EQ(0ULL, Measure::GetCount());

        SharedPtr<Phrase> phrase = make<Phrase>();

        for (uint i = 0; i < 8; ++i)
        {
            phrase->measures.emplace_back(make<Measure>());
        }
        const Vector<WeakPtr<HarmonyNode>>& progression = Random::in(graph->progressions);

        applyProgression(phrase, progression);

        uint expectedHarmonyNodesCount = static_cast<uint>(progression.size());
        uint actualHarmonyNodesCount = 0;

        for (SharedPtr<Measure>& measure : phrase->measures)
        {
            actualHarmonyNodesCount += static_cast<uint>(measure->progression.size());
        }

        if (expectedHarmonyNodesCount != actualHarmonyNodesCount)
        {
            FAIL() << "failed during epoch " << n << ": expected " << expectedHarmonyNodesCount
                << " harmony_nodes in measures but found " << actualHarmonyNodesCount;
        }

        EXPECT_EQ(1ULL, Phrase::GetCount());
        EXPECT_EQ(8ULL, Measure::GetCount());
    }
}

TEST_F(DryadTests, ApplyMotifToPhrase)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    uint epoch = 100;
    SharedPtr<Voice> voice = make<Voice>();

    for (uint n = 0; n < epoch; ++n)
    {
        EXPECT_EQ(Note::GetCount(), 0ULL);

        SharedPtr<Phrase> phrase = make<Phrase>();

        uint measureCount = Random::in(Vector<uint>{4, 8, 16});

        for (uint i = 0; i < measureCount; ++i)
        {
            appendMeasure(phrase, make<Measure>());
        }

        SharedPtr<MotifVariation> motif = make<MotifVariation>();
        SharedPtr<MotifConfig> motifConfig = make<MotifConfig>();

        uint duration = motifConfig->duration = Random::in(Vector<uint>{Constants::Duration::Half, Constants::Duration::HalfDotted, Constants::Duration::Whole, Constants::Duration::WholeDotted});
        motifConfig->rhythmicEnergy = Random::range(duration / Constants::Duration::Half, duration / Constants::Duration::Sixteenth);
        motifConfig->melodicEnergy = Random::range(0, 100);
        uint min = motifConfig->minMelodicEnergy = Random::range(0U, 4U);
        motifConfig->maxMelodicEnergy = Random::range(min, 16ULL);

        generateMotif(motif, motifConfig);

        applyMotif(phrase, motif, voice);

        // evaluate with position count and presence of voice and note duration values
        uint cumulativeDurationOfNotes = 0;

        for (SharedPtr<Measure> measure : phrase->measures)
        {
            for (SharedPtr<Position> position : measure->positions)
            {
                EXPECT_EQ(1ULL, position->notes.size()) << "failed during epoch " << n 
                    << ": each position should only contain one note, this one contains " << position->notes.size();

                SharedPtr<Note>& note = position->notes[0];

                if (note->voice == nullptr)
                {
                    FAIL() << "failed during epoch " << n << ": Note should be assigned to a voice";
                }

                cumulativeDurationOfNotes += note->duration;
            }
        }

        uint expectedDuration = measureCount * Constants::Duration::Whole;
        if (cumulativeDurationOfNotes != expectedDuration)
        {
            FAIL() << "failed during epoch " << n << ": expected total duration of " << expectedDuration
                << " but actual value is " << cumulativeDurationOfNotes;
        }
    }
}

TEST_F(DryadTests, ApplyScaleToScore)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    uint epoch = 100;

    SharedPtr<Score> score = createScore();
    score->graph = createMajorGraph();
    generateAllProgressions(score->graph);
    SharedPtr<Scale> scale = createMajorScale();

    for (uint n = 0; n < epoch; ++n)
    {
        EXPECT_EQ(Phrase::GetCount(), 0ULL);
        EXPECT_EQ(Measure::GetCount(), 0ULL);
        EXPECT_EQ(Position::GetCount(), 0ULL);
        EXPECT_EQ(Note::GetCount(), 0ULL);

        Vector<uint> counts = {2, 4, 8, 16};
        uint phraseCount = Random::in(counts);
        uint measuresByPhrase = Random::in(counts);

        for (uint i = 0; i < phraseCount; ++i)
        {
            appendPhrase(score, make<Phrase>());
        }

        SharedPtr<MotifVariation> motif = make<MotifVariation>();

        SharedPtr<MotifConfig> motifConfig = make<MotifConfig>();
        uint duration = motifConfig->duration = Random::in(Vector<uint>{Constants::Duration::Half, Constants::Duration::HalfDotted, Constants::Duration::Whole, Constants::Duration::WholeDotted});
        motifConfig->rhythmicEnergy = Random::range(duration / Constants::Duration::Half, duration / Constants::Duration::Sixteenth);
        motifConfig->melodicEnergy = Random::range(0, 100);
        uint min = motifConfig->minMelodicEnergy = Random::range(0, 4);
        motifConfig->maxMelodicEnergy = Random::range(min, 16ULL);

        generateMotif(motif, motifConfig);

        SharedPtr<Voice> voice = make<Voice>();

        for (SharedPtr<Phrase> phrase : score->phrases)
        {
            for (uint i = 0; i < measuresByPhrase; ++i)
            {
                appendMeasure(phrase, make<Measure>());
            }
        }

        for (SharedPtr<Phrase> phrase : score->phrases)
        {
            applyProgression(phrase, Random::in(score->graph->progressions));
            applyMotif(phrase, motif, voice);
        }

        SharedPtr<ScaleConfig> scale_config = make<ScaleConfig>(Random::range(0, 11), (Accidental)Random::range(0, 2));

        applyScale(score, scale, scale_config);

        SharedPtr<Position> scorePosition = score->phrases[0]->measures[0]->positions[0];

        while (scorePosition != nullptr)
        {
            if (scorePosition->notes.size() == 0)
            {
                FAIL() << "failed during epoch " << n << ": a position was created without notes";
            }

            for (SharedPtr<Note> note : scorePosition->notes)
            {
                if (note->midi < 0)
                {
                    FAIL() << "failed during epoch " << n << ": a note was created without midi value";
                }
                else if (note->duration <= 0)
                {
                    FAIL() << "failed during epoch " << n << ": a note was created with an invalid duration";
                }
                else if (note->octave < 0)
                {
                    FAIL() << "failed during epoch " << n << ": a note was created without an octave value";
                }
            }

            scorePosition = next(scorePosition);
        }

        score->phrases.clear();
    }
}

TEST_F(DryadTests, CreateAValidMajorScale)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    SharedPtr<Scale> scale = createMajorScale();
    EXPECT_EQ(scale->degrees.size(), 7) << "Scale has wrong size";

    EXPECT_EQ(scale->degrees[0]->intervalFromRoot, 0) << "Wrong inteval for I";
    EXPECT_EQ(scale->degrees[1]->intervalFromRoot, 2) << "Wrong inteval for ii";
    EXPECT_EQ(scale->degrees[2]->intervalFromRoot, 4) << "Wrong inteval for iii";
    EXPECT_EQ(scale->degrees[3]->intervalFromRoot, 5) << "Wrong inteval for IV";
    EXPECT_EQ(scale->degrees[4]->intervalFromRoot, 7) << "Wrong inteval for V";
    EXPECT_EQ(scale->degrees[5]->intervalFromRoot, 9) << "Wrong inteval for vi";
    EXPECT_EQ(scale->degrees[6]->intervalFromRoot, 11) << "Wrong inteval for viid";

    SharedPtr<Degree> I = scale->degrees[0];
    SharedPtr<Degree> I_again = next(next(next(next(next(next(next(I)))))));
    EXPECT_EQ(I, I_again) << "Degrees are not linked correctly";

    SharedPtr<Degree> ii = scale->degrees[1];
    SharedPtr<Degree> ii_again = previous(previous(previous(previous(previous(previous(previous(ii)))))));

    EXPECT_EQ(ii, ii_again) << "Degrees are not linked correctly";
    EXPECT_EQ(7ULL, Degree::GetCount());
}

TEST_F(DryadTests, GenerateMajorGraphProgressions)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    SharedPtr<HarmonyGraph> graph = createMajorGraph();
    generateAllProgressions(graph);

    EXPECT_EQ(graph->progressions.size(), 1384) << "1384 permutations are expected from default major mode";
}

TEST_F(DryadTests, SpendMelodicEnergyCorrectly)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    SharedPtr<Motif> motif = make<Motif>();
    SharedPtr<MotifConfig> motifConfig = make<MotifConfig>();

    uint epoch = 100;
    uint noteCount = 8;

    for (uint n = 0; n < epoch; ++n)
    {
        motif->variations.emplace_back(make<MotifVariation>());
        SharedPtr<MotifVariation> variation = motif->variations[n];

        motifConfig->duration = 2 * Constants::Duration::Whole;
        Energy minEnergy = motifConfig->minMelodicEnergy = Random::range(1, 3);
        Energy maxEnergy = motifConfig->maxMelodicEnergy = Random::range(minEnergy + 1, Energy(12));

        Energy totalEnergy = motifConfig->melodicEnergy = Random::range(Energy(noteCount * minEnergy / 2), Energy(noteCount * maxEnergy * 1.25));

        for (uint i = 0; i < noteCount; ++i)
        {
            variation->notes.emplace_back(make<Note>())->duration = Constants::Duration::Quarter;
        }

        spendMelodicEnergy(variation, motifConfig);

        Energy spentEnergy =
            std::reduce
            (
                variation->notes.begin(),
                variation->notes.end(),
                Energy(0),
                [](Energy acc, const SharedPtr<Note>& note)
                {
                    return acc + std::abs(note->offset);
                }
            );

        Energy expectedEnergySpent = std::min(totalEnergy, Energy(noteCount * maxEnergy));

        if (spentEnergy != expectedEnergySpent)
        {
            FAIL() << "failed during epoch " << n << ": energy spent is " << spentEnergy << " while expected spending was " << expectedEnergySpent;
        }
    }
}

TEST_F(DryadTests, SpendRythmicEnergyCorrectly)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    SharedPtr<Motif> motif = make<Motif>();
    SharedPtr<MotifConfig> motifConfig = make<MotifConfig>();

    uint epoch = 100;

    for (uint n = 0; n < epoch; ++n)
    {
        motif->variations.emplace_back(make<MotifVariation>());
        SharedPtr<MotifVariation> variation = motif->variations[n];

        motifConfig->duration = 2 * Constants::Duration::Whole;
        uint maxNotes = motifConfig->duration / Constants::Duration::Sixteenth;

        uint rhythmicEnergy = motifConfig->rhythmicEnergy = Random::range(0ULL, maxNotes);

        spendRythmicEnergy(variation, motifConfig);

        uint expectedNoteCount = std::min(rhythmicEnergy + 1, maxNotes);
        uint actualNoteCount = (uint)variation->notes.size();

        if (actualNoteCount != expectedNoteCount)
        {
            FAIL() << "failed during epoch " << n << ": should have splitted motif in " << expectedNoteCount << " notes but made " << actualNoteCount;
        }
    }
}

TEST_F(DryadTests, EmptyTest_DISABLED)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    uint epoch = 100;

    for (uint n = 0; n < epoch; ++n)
    {
        if (false)
        {
            FAIL() << "failed during epoch " << n << ": ";
        }
    }
}

} // namespace Dryad
