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
    Score& score = session.getScore();
    Phrase phrase(score);

    Phrase* previousPhrase = phrase.prev();
    EXPECT_EQ(previousPhrase, nullptr);

    EXPECT_FALSE(score.hasChanged());
    Phrase& phrase1 = score.appendChild();
    EXPECT_TRUE(score.hasChanged());
    Phrase& phrase2 = score.appendChild({score});
    Phrase& phrase3 = phrase2.append();
    Phrase& phrase4 = phrase3.append({score});

    EXPECT_EQ(phrase1.next(), &phrase2);
    EXPECT_EQ(phrase2.prev(), &phrase1);
    EXPECT_EQ(&(phrase2.getScore()), &score);
    EXPECT_EQ(&(phrase2.getSession()), &session);
}

} // namespace Dryad
