#include "./test_common.hpp"


TEST(NDA, MinMaxElement) {

  nda::array<int,2> A(3, 3), B(3, 3), C;

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      A(i, j) = i + 2 * j + 1;
      B(i, j) = i - 3 * j;
    }

  C = A + B;

  EXPECT_EQ(max_element(A), 7);
  EXPECT_EQ(min_element(A), 1);
  EXPECT_EQ(max_element(B), 2);
  EXPECT_EQ(min_element(B), -6);
  EXPECT_EQ(max_element(A + B), 5);
  EXPECT_EQ(min_element(A + B), -1);
  EXPECT_EQ(sum(A), 36);
}

// ==============================================================

TEST(NDA, Map) {

  nda::array<double,2> A(3, 3), B(3, 3), Sqr_A(3, 3), abs_B_B(3, 3), A_10_m_B(3, 3), abs_A_10_m_B(3, 3), max_A_10_m_B(3, 3), pow_A(3, 3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      A(i, j) = i + 2 * j + 1;
      B(i, j) = i - 3 * j;

      pow_A(i, j)        = A(i, j) * A(i, j);
      Sqr_A(i, j)        = A(i, j) * A(i, j);
      abs_B_B(i, j)      = std::abs(2 * B(i, j));
      A_10_m_B(i, j)     = A(i, j) + 10 * B(i, j);
      abs_A_10_m_B(i, j) = std::abs(A(i, j) + 10 * B(i, j));
      max_A_10_m_B(i, j) = std::max(A(i, j), 10 * B(i, j));
    }

  auto Abs = nda::map([](double x) { return std::fabs(x); });
  auto Max = nda::map([](double x, double y) { return std::max(x, y); });
  auto sqr = nda::map([](double x) { return x * x; });

  EXPECT_ARRAY_NEAR(nda::array(pow(nda::array_view<double, 2>{A}, 2)), Sqr_A);
  EXPECT_ARRAY_NEAR(nda::array(sqr(A)), Sqr_A);
  EXPECT_ARRAY_NEAR(nda::array(Abs(B + B)), abs_B_B);
  EXPECT_ARRAY_NEAR(nda::array(A + 10 * B), A_10_m_B);
  EXPECT_ARRAY_NEAR(nda::array(Abs(A + 10 * B)), abs_A_10_m_B);
  EXPECT_ARRAY_NEAR(nda::array(Max(A, 10 * B)), max_A_10_m_B);
}

// ==============================================================

MAKE_MAIN
