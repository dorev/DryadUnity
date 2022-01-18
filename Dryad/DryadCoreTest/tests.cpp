#include "gtest/gtest.h"
#include "dryad/session.h"
#include "dryad/score/phrase.h"

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

TEST_F(DryadTests, Session)
{
    Session session;
    Score& score = session.GetScore();
    Phrase phrase(score);

    Phrase* previousPhrase = phrase.Prev();
    EXPECT_EQ(previousPhrase, nullptr);

    EXPECT_FALSE(score.HasChanged());
    Phrase& phrase1 = score.AppendChild();
    EXPECT_TRUE(score.HasChanged());
    Phrase& phrase2 = score.AppendChild({score});
    Phrase& phrase3 = phrase2.Append();
    Phrase& phrase4 = phrase3.Append({score});

    EXPECT_EQ(phrase1.Next(), &phrase2);
    EXPECT_EQ(phrase2.Prev(), &phrase1);
    EXPECT_EQ(&(phrase2.GetScore()), &score);
    EXPECT_EQ(&(phrase2.GetSession()), &session);
}

} // namespace Dryad
