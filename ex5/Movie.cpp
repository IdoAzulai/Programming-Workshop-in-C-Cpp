
#include "Movie.h"
#define HASH_START 17
#define RES_MULT 31

/// Constructor
Movie::Movie(const string &movie_name, int movie_year): name(movie_name),
                                                        year(movie_year) {}

/// Movie name getter
string Movie::get_name() const
{
  return name;
}

/// Movie year getter
int Movie::get_year() const
{
  return year;
}

/// returns false if (movies are the same, or rhs is totally bigger than lhs)
/// true if lhs is totally bigger than rhs
bool Movie::operator<(const Movie& rhs) const
{
  return year < rhs.year || (year <= rhs.year && name < rhs.name);
}


/// prints a movie by the format
ostream &operator<<(ostream &os, const Movie &movie)
{
  return os << movie.name << "(" << movie.year << ")" << endl;
}


/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash(const sp_movie& movie){
  std::size_t res = HASH_START;
  res = res * RES_MULT + std::hash<std::string>()(movie->get_name());
  res = res * RES_MULT + std::hash<int>()(movie->get_year());
  return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal(const sp_movie& m1, const sp_movie& m2){
  return !(*m1 < *m2) && !(*m2 < *m1);
}
