-- 1.sql: Titles of all movies released in 2008
SELECT title
FROM movies
WHERE year = 2008;

-- 2.sql: Birth year of Emma Stone
SELECT birth
FROM people
WHERE name = 'Emma Stone';

-- 3.sql: Titles of all movies released on or after 2018, alphabetical
SELECT title
FROM movies
WHERE year >= 2018
ORDER BY title;

-- 4.sql: Number of movies with an IMDb rating of 10.0
SELECT COUNT(*) 
FROM ratings
WHERE rating = 10.0;

-- 5.sql: Titles and release years of all "Harry Potter" movies, chronological
SELECT title, year
FROM movies
WHERE title LIKE 'Harry Potter%'
ORDER BY year;

-- 6.sql: Average rating of all movies released in 2012
SELECT AVG(r.rating)
FROM ratings r
JOIN movies m ON r.movie_id = m.id
WHERE m.year = 2012;

-- 7.sql: Movies released in 2010 and their ratings, by rating desc then title asc
SELECT m.title, r.rating
FROM movies m
JOIN ratings r ON m.id = r.movie_id
WHERE m.year = 2010
ORDER BY r.rating DESC, m.title ASC;

-- 8.sql: Names of all people who starred in Toy Story
SELECT DISTINCT p.name
FROM people p
JOIN stars s ON p.id = s.person_id
WHERE s.movie_id = (SELECT id FROM movies WHERE title = 'Toy Story');

-- 9.sql: Names of people who starred in a movie released in 2004, ordered by birth year
SELECT DISTINCT p.name
FROM people p
JOIN stars s ON p.id = s.person_id
JOIN movies m ON s.movie_id = m.id
WHERE m.year = 2004
  AND p.birth IS NOT NULL
ORDER BY p.birth;

-- 10.sql: Names of people who have directed a movie with rating >= 9.0
SELECT DISTINCT p.name
FROM people p
JOIN directors d ON p.id = d.person_id
JOIN ratings r ON d.movie_id = r.movie_id
WHERE r.rating >= 9.0;

-- 11.sql: Titles of the five highest-rated movies that Chadwick Boseman starred in
SELECT m.title
FROM movies m
JOIN ratings r ON m.id = r.movie_id
JOIN stars s ON m.id = s.movie_id
JOIN people p ON s.person_id = p.id
WHERE p.name = 'Chadwick Boseman'
ORDER BY r.rating DESC
LIMIT 5;

-- 12.sql: Titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred
SELECT m.title
FROM movies m
WHERE m.id IN (
    SELECT movie_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name = 'Bradley Cooper')
)
AND m.id IN (
    SELECT movie_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name = 'Jennifer Lawrence')
);

-- 13.sql: Names of all people who starred in a movie in which Kevin Bacon (born 1958) also starred (exclude Kevin Bacon)
SELECT DISTINCT p.name
FROM people p
JOIN stars s ON p.id = s.person_id
WHERE s.movie_id IN (
    SELECT movie_id
    FROM stars
    WHERE person_id = (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958)
)
AND p.id != (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958);
