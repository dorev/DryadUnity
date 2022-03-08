#include "gtest/gtest.h"
#include "dryad/session.h"

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
}

} // namespace Dryad
