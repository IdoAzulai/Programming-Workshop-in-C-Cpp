

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


/// Constructor
/// TODO: do i need to deepcopy rankmap? if so, a for loop
/// TODO: that copies each key:val from rank_map_in
User::User(const string& name,
           const rank_map &rank_map_in,
           std::shared_ptr<RecommendationSystem> &rec_sys_input):
user_name(name),
user_rank_map(rank_map_in),
rec_sys(rec_sys_input)
{}

void User::add_movie_to_rs(const string &name, int year,
                           const vector<double> &features,
                           double rate)
{
    if (rate < MIN_RATE || rate > MAX_RATE || std::isnan(rate)){
        //name = nullptr || year < 0 || features = nullptr
        return;
    }
    sp_movie new_m = rec_sys->add_movie(name, year, features);
    user_rank_map[new_m] = rate;
}

/// return the username
string User::get_name() const
{
    return user_name;
}

/// returns user's rank map
rank_map User::get_rank() const
{
    return user_rank_map;
}

/// returns a pointer to a recommended movie by the algorithm
sp_movie User::get_recommendation_by_content() const
{
    return rec_sys->get_recommendation_by_content(*this);
}

/// returns a recommendation according to the similarity recommendation method
sp_movie User::get_recommendation_by_cf(int k) const
{
  return rec_sys->recommend_by_cf (*this, k);
}

/// predicts the score for a given movie
double User::get_prediction_score_for_movie(const string &name,
                                            int year, int k) const
{
    sp_movie movie_ptr = rec_sys->get_movie(name, year);
    return rec_sys->predict_movie_score(*this, movie_ptr, k);
}

/// output stream operator (friend)
ostream &operator<< (ostream &os, const User &user){
    return os << user.user_name << endl << *user.rec_sys << endl;
}
