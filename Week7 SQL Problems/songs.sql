-- 1. List all song names
SELECT name
FROM songs;

-- 2. List song names in increasing order of tempo
SELECT name
FROM songs
ORDER BY tempo;

-- 3. List top 5 longest songs, in descending order of length
SELECT name
FROM songs
ORDER BY duration_ms DESC
LIMIT 5;

-- 4. Songs with danceability, energy, and valence > 0.75
SELECT name
FROM songs
WHERE danceability > 0.75
  AND energy > 0.75
  AND valence > 0.75;

-- 5. Average energy of all songs
SELECT AVG(energy)
FROM songs;

-- 6. Songs by Post Malone
SELECT name
FROM songs
WHERE artist_id = (
    SELECT id
    FROM artists
    WHERE name = 'Post Malone'
);

-- 7. Average energy of songs by Drake
SELECT AVG(energy)
FROM songs
JOIN artists ON songs.artist_id = artists.id
WHERE artists.name = 'Drake';

-- 8. Songs that feature other artists (“feat.” in name)
SELECT name
FROM songs
WHERE name LIKE '%feat.%';
