# Lonpos solver

Hi!

So, I wrote this to solve a game called *Lonpos* (lonpos.cc).
This meet-in-the middle algorithm treats the game grid as a 9x9 square, where some
cells are permanently occupied. The cells that are occupied by already-placed pieces
are treated in exactly the same way. Firstly, the algorithm finds the initially-free
cells and numbers them. Then, for every existing piece, it generates every possible
placement that occupies only initially-free cells. Every placement is converted into
a bitmask, that shows, which initially-occupied cells are occupied for this placement.
Then, the algorithm divides the pieces into two groups ("meet in the middle"). For
every group it generates all possible placements. When processing the right group,
the algorithm checks whether a complementary configuration was created using the
left group. If so, the two placements actually constitute a valid solution. After
processing the groups, one valid placement for the first group is shown. This
makes it quite easy to get the actual solution by trial and error.

Of course, there are optimisations to be had, pieces could be specified in
correct order, the complete solution could be visualized etc., but this
code did the job I wrote it for and I would like to move on.

See you!
