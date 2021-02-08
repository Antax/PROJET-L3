#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

TEST(test, test) {
  sp::Shared<int> shared;
  sp::Weak<int> weak;
  sp::Unique<int> unique;
}

TEST(UNIQUE,constructorNoArgs){
  sp::Unique <int> unique;
  EXPECT_FALSE(unique.get());
}

TEST(UNIQUE,constructorWithArgs){
  sp::Unique <int> unique(new int(4));
  EXPECT_TRUE(*unique ==  4);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
