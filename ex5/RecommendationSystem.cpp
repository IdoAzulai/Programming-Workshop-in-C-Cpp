//
// Created by ido.azulai on 3/24/24.
//
#include "RecommendationSystem.h"

using std::make_shared;
using std::transform;
using std::sqrt;

// Comparison function for movies
bool
RecommendationSystem::movies_less_than (sp_movie m1, sp_movie m2)
{
  return *m1 < *m2;
}

RecommendationSystem::RecommendationSystem (): rec_sys_ins(movies_less_than){}

sp_movie RecommendationSystem::add_movie(const string &name,
                                         int year,
                                         const vector<double> &features)
{
  sp_movie movie_to_add = make_shared<Movie>(name, year);
  rec_sys_ins[movie_to_add] = features;
  return movie_to_add;
}

/// Calculates average of rankings in an unordered map
double avg_ranks(const rank_map& user_rankings)
{
  double sum = 0;
  for (const auto& it : user_rankings)
  {
    sum += it.second;
  }
  if (sum == 0)
  {
    return 0;
  }
  return sum / user_rankings.size();
}


/// Function to add two vectors element-wise
vector<double>& operator+=(vector<double>& lhs, const vector<double>& rhs)
{
  // Ensure both vectors have the same size
  if (lhs.size() != rhs.size())
  {
    throw std::invalid_argument("Vectors must have "
                                "the same size for element-wise addition.");
  }

  // Element-wise addition
  for (size_t i = 0; i < lhs.size(); ++i)
  {
    lhs[i] += rhs[i];
  }
  return lhs;
}

/// creates the pref vector by adding each normalized movie vec to pref vec
vector<double>
RecommendationSystem::calc_pref_vec (const User &user_rankings, double avg)
{
  // normalize the rankings map (original minus avg), and add to pref vector
  // init pref vector
  vector<double> pref_vec(rec_sys_ins.begin()->second.size(), 0);
  for (const auto& it : user_rankings.get_rank())
  {
    /// TODO check if normalized_ranks[it.first] is not NA
    /// TODO if so, calc normalized value and mult in the specific
    /// movie's vector, and add it to pref_vec
    // hold scalar value
    double scalar = it.second - avg;

    // mult scalar and movie's ranking vector manually
    // and update pref vector by element-wise addition manually
    for (size_t i = 0; i < rec_sys_ins[it.first].size(); ++i)
    {
      pref_vec[i] += rec_sys_ins[it.first][i] * scalar;
    }
  }
  return pref_vec;
}


/// Calculate the dot product
double vec_dot(const vector<double>& v1, const vector<double>& v2)
{
  double result = 0.0;
  for (size_t i = 0; i < v1.size (); ++i)
  {
    result += v1[i] * v2[i];
  }
  return result;
}

/// Function to calculate the norm (magnitude) of a vector
double vector_norm(const vector<double>& vec)
{
  double sum_of_squares = 0.0;
  for (double value : vec) {
    sum_of_squares += value * value;
  }
  return sqrt(sum_of_squares);
}

/// Checks if user already ranked this movie (from rec_sys)
bool RecommendationSystem::has_rank(const User& user, const sp_movie& movie)
{
  try
  {
    // Attempt to retrieve the ranking for the movie from the user
    user.get_rank().at(movie);

    // If the ranking retrieval didn't throw an exception,
    // it means the user has ranked the movie
    return true;
  }
  catch (const std::out_of_range&)
  {
    // If the movie is not found in the user's rankings, return false
    return false;
  }
}

/// calc the angle between each movie and return a sp_movie of the closest
sp_movie
RecommendationSystem::find_most_similar(const User &user_rankings,
                                        const vector<double>& pref_vec)
{
  double max_angle = -1.0; // Any angle calculated will be greater than this
  double v1_size = vector_norm(pref_vec);
  sp_movie closest_movie = nullptr; // Initialize with nullptr

  // Iterate over the movies the user didn't watch
  for (const auto& movie_entry : rec_sys_ins)
  {
    // Check if the user has not ranked this movie
    if (!has_rank(user_rankings, movie_entry.first))
    {
      // Calculate dot product and vector size for the current movie
      double dot_product = vec_dot(pref_vec, movie_entry.second);
      double v2_size = vector_norm(movie_entry.second);

      // Calculate angle between preference vector and current movie's vector
      double angle = dot_product / (v1_size * v2_size);

      // Update closest movie if the current movie has a higher angle
      if (angle > max_angle)
      {
        max_angle = angle;
        closest_movie = movie_entry.first;
      }
    }
  }
  return closest_movie;
}

/// Calculates the closest movie for user, by rating similarity
sp_movie RecommendationSystem::
get_recommendation_by_content(const User &user_rankings)
{
  // calc average ranking of all movies the user ranked
  double avg = avg_ranks(user_rankings.get_rank()); // calc average

  // normalize the rankings map (original minus avg), and add to pref vector
  vector<double> pref_vec = calc_pref_vec(user_rankings, avg);

  // calculate the difference between pref_vec and other movies vectors
  return find_most_similar(user_rankings, pref_vec);
}

///calc angle between 2 movies, for the cf recommendation system
double RecommendationSystem::calc_angle (const vector<double> &watched,
                                         const vector<double> &possible_movie)
{
  double v1_size = vector_norm(watched); // pre-calc the norm
  double v2_size = vector_norm(possible_movie);
  double dot_product = vec_dot(watched, possible_movie);

  return dot_product / (v1_size * v2_size);
}

/// predicts a score for a movie, based on user's rank map
double RecommendationSystem::predict_movie_score (const User &user,
                                                  const sp_movie &movie,int k)
{
  // calculate angle between each movie user didn't watch to all movies he did
  map<double, sp_movie> movie_recs; // an ordered map, holds all angles
  double top = 0.0, base = 0.0;
  int cnt = 0;

  for (const auto& watched_movie : user.get_rank())
  {
    // calc angle
    if (watched_movie.first != movie){
      double angle = calc_angle(rec_sys_ins[watched_movie.first],
                                rec_sys_ins[movie]);
      movie_recs[angle] = watched_movie.first;
      cnt++;
    }
  }

  // calculate predicted rank for the movie we got, for k top user rating's
  auto possible_rank = movie_recs.end();
  --possible_rank;
  k = (k > cnt) ? cnt : k; // if k is bigger than amount of movies user ranked
  for (int i = 0; i < k; ++i)
  {
    top += possible_rank->first *
           user.get_rank()[possible_rank->second];
    base += possible_rank->first;
    --possible_rank;
  }

  // calc rank (sum(similar * rank) / sum(similar))
  return top/base;
}

/// recommends a movie from the movies the user didn't watch, by cf
sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k)
{
  double max_rate = 0.0, angle = 0.0;
  sp_movie recommended_movie = nullptr;
  for (const auto& it : rec_sys_ins)
  {
    try {
      user.get_rank().at (it.first); // look for the sp_movie in user's ranks
    }
    catch (const std::out_of_range& e) {
      // means we have in it.first a movie the user didn't rate
      angle = predict_movie_score (user, it.first, k);
      if (angle > max_rate)
      {
        max_rate = angle;
        recommended_movie = it.first;
      }
    }
  }

  // return the sp+movie that got the highest score
  return recommended_movie;
}


///// return a smart pointer to a movie, by it's name and year
sp_movie RecommendationSystem::get_movie(const string& name, int year) const
{
  sp_movie movie = make_shared<Movie>(name, year);

  // Loop through each entry in rec_sys_ins to find the movie
  for (const auto& pair : rec_sys_ins)
  {
    const sp_movie& current_movie = pair.first;
    if (current_movie->get_name() == name && current_movie->get_year() == year)
    {
      // Movie found, return the smart pointer to the movie
      return current_movie;
    }
  }

  // Movie not found, return nullptr
  return nullptr;
}

/// output stream operator (friend)
ostream &operator<< (ostream &os, const RecommendationSystem &sys)
{
  for (const auto &it : sys.rec_sys_ins)
  {
    os << *it.first;
  }
  return os;
}