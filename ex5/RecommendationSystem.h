//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include "User.h"

using std::map;

typedef bool (*movies_less_than)(sp_movie, sp_movie);
typedef map<sp_movie, vector<double>, movies_less_than> rec_sys;

class RecommendationSystem
{
 private:
  rec_sys rec_sys_ins;

  // comperator for sp_movie in the map so it would know how to order it
  static bool movies_less_than(sp_movie m1, sp_movie m2);

  vector<double> calc_pref_vec(const User &user_rankings, double avg);

  sp_movie find_most_similar(const User &user_rankings,
                             const vector<double> &pref_vec);

  double calc_angle (const vector<double> &watched,
                     const vector<double> &possible_movie);

  static bool has_rank (const User &user, const sp_movie &movie);

 public:

  /**
   * Default constructor, initializes an empty map.
   */
  RecommendationSystem();

  /**
   *
   * @param user_rankings
   * @return
   */
  sp_movie get_recommendation_by_content (const User& user_rankings);

  //explicit RecommendationSystem()
  /**
   * adds a new movie to the rec_sys_ins
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in rec_sys_ins
   */
  sp_movie add_movie(const string& name,int year,
                     const vector<double>& features);


  /**
   * a function that calculates the movie with highest score
   * based on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in rec_sys_ins
   */
  sp_movie recommend_by_content(const User& user);

  /**
   * a function that calculates the movie with highest predicted score
   * based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in rec_sys_ins
   */
  sp_movie recommend_by_cf(const User& user, int k);


  /**
   * Predict a user rating for a movie given argument using item cf
   * procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score(const User &user, const sp_movie &movie,
                             int k);

  /**
   * gets a shared pointer to movie in rec_sys_ins
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in rec_sys_ins
   */
  sp_movie get_movie(const string &name, int year) const;


  friend ostream &operator<< (ostream &os, const RecommendationSystem &sys);

};


#endif //RECOMMENDATIONSYSTEM_H
