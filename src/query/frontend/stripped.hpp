#pragma once

#include <string>
#include <unordered_map>

#include "query/parameters.hpp"
#include "utils/hashing/fnv.hpp"

namespace query {

// Strings used to replace original tokens. Different types are replaced with
// different token.
const std::string kStrippedIntToken = "0";
const std::string kStrippedDoubleToken = "0.0";
const std::string kStrippedStringToken = "\"a\"";
const std::string kStrippedBooleanToken = "true";

/**
 * StrippedQuery contains:
 *     * stripped query
 *     * literals stripped from query
 *     * hash of stripped query
 */
class StrippedQuery {
 public:
  /**
   * Strips the input query and stores stripped query, stripped arguments and
   * stripped query hash.
   *
   * @param query Input query.
   */
  explicit StrippedQuery(const std::string &query);

  /**
   * Copy constructor is deleted because we don't want to make unnecessary
   * copies of this object (copying of string and vector could be expensive)
   */
  StrippedQuery(const StrippedQuery &other) = delete;
  StrippedQuery &operator=(const StrippedQuery &other) = delete;

  /**
   * Move is allowed operation because it is not expensive and we can
   * move the object after it was created.
   */
  StrippedQuery(StrippedQuery &&other) = default;
  StrippedQuery &operator=(StrippedQuery &&other) = default;

  const std::string &query() const { return query_; }
  const auto &original_query() const { return original_; }
  const auto &literals() const { return literals_; }
  const auto &named_expressions() const { return named_exprs_; }
  const auto &parameters() const { return parameters_; }
  HashType hash() const { return hash_; }

 private:
  // Return len of matched keyword if something is matched, otherwise 0.
  int MatchKeyword(int start) const;
  int MatchString(int start) const;
  int MatchSpecial(int start) const;
  int MatchDecimalInt(int start) const;
  int MatchOctalInt(int start) const;
  int MatchHexadecimalInt(int start) const;
  int MatchReal(int start) const;
  int MatchParameter(int start) const;
  int MatchEscapedName(int start) const;
  int MatchUnescapedName(int start) const;
  int MatchWhitespaceAndComments(int start) const;

  // Original query.
  std::string original_;

  // Stripped query.
  std::string query_;

  // Token positions of stripped out literals mapped to their values.
  // TODO: Parameters class really doesn't provided anything interesting. This
  // could be changed to std::unordered_map, but first we need to rewrite (or
  // get rid of) hardcoded queries which expect Parameters.
  Parameters literals_;

  // Token positions of query parameters mapped to theirs names.
  std::unordered_map<int, std::string> parameters_;

  // Token positions of nonaliased named expressions in return statement mapped
  // to theirs original/unstripped string.
  std::unordered_map<int, std::string> named_exprs_;

  // Hash based on the stripped query.
  HashType hash_;
};
}  // namespace query
