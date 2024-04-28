//
// Created on 2/20/2022.
//

#ifndef USER_H
#define USER_H
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "Movie.h"
#include <algorithm>
#include <cmath>

#define MAX_RATE 10
#define MIN_RATE 1

using std::unordered_map;

class RecommendationSystem;
typedef unordered_map<sp_movie, double, hash_func, equal_func> rank_map;

using std::string;

class User
{
 private:
  string user_name;
  rank_map user_rank_map;
  std::shared_ptr<RecommendationSystem> rec_sys;

 public:

  /**
   * Constructor for the class
   */
  User(const string& name, const rank_map &rank_map_in,
       std::shared_ptr<RecommendationSystem> &rec_sys_input);

  /**
   * function for adding a movie to the DB
   * @param name name of movie
   * @param year year it was made
   * @param features a vector of the movie's features
   * @param rate the user rate for this movie
   */
  void add_movie_to_rs(const string &name, int year,
                       const vector<double> &features,
                       double rate);

  /**
   * a getter for the user's name
   * @return the username
   */
  string get_name() const;


  /**
   * a getter for the ranks map
   * @return the user's rank map
   */
  rank_map get_rank() const;

  /**
   * returns a recommendation according to the movie's content
   * @return recommendation
   */
  sp_movie get_recommendation_by_content() const;

  /**
   * returns a recommendation according to the similarity
   * recommendation method
   * @param k the number of the most similar movies to calculate by
   * @return recommendation
   */
  sp_movie get_recommendation_by_cf(int k) const;

  /**
   * predicts the score for a given movie
   * @param name the name of the movie
   * @param year the year the movie was created
   * @param k the parameter which represents the number of the most
   * similar movies to predict the score by
   * @return predicted score for the given movie
   */
  double get_prediction_score_for_movie(const string& name,
                                        int year, int k) const;

  /**
   * output stream operator
   * @param os the output streamRecommendationSystem
   * @param user the user
   * @return output stream
   */
  friend ostream &operator<< (ostream &os, const User &user);
};



#endif //USER_H
