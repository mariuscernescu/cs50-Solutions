#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        string namei = candidates[i];
        if (strcmp(namei, name) == 0)
        {
            ranks[rank] = i;
            // printf("ranks[%i] = %i\n", rank, i);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) // De de reparat. Problema cu 0.
{
    int j;
    int i;
    for (i = 0; i < candidate_count - 1; i++)
    {
        if (true)
        {
            for (j = 0 ; j < candidate_count - i; j++)
            {
                if (ranks[i] != ranks[j + i])
                {
                    preferences[ranks[i]][ranks[j + i]]++;
                    //printf("Preference[%i][%i] = %i \n ", ranks[i], ranks[j + i], preferences[ranks[i]][ranks[j + i]]);
                }

            }
        }



    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i]) // If one candidate is prefered
            {
                pairs[pair_count].winner = i; // Add that candidate to the winer
                pairs[pair_count].loser = j;  // Add the other one to the loser
                pair_count++;        // Increase pair count
                //printf("if %i > %i \n", preferences[i][j], preferences[j][i]);


            }

            // printf("Pair count = %i\n", pair_count);
        }
    }
    //printf("Winners = %i > %i > %i\n", pairs[0].winner,  pairs[1].winner,  pairs[2].winner);
    //printf("Losers = %i > %i > %i\n", pairs[0].loser,  pairs[1].loser,  pairs[2].loser);
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int counter = 0;
    int strong[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                strong[counter] = preferences[i][j] - preferences[j][i];
                counter++;

            }
        }
    }
    //printf("Strong = %i> %i> %i\n", strong[0], strong[1], strong[2]);


    pair swap;
    int swapy;
    for (int c = 0 ; c < pair_count - 1; c++)
    {
        for (int d = 0 ; d < pair_count - c - 1; d++)
        {
            if (strong[d] < strong[d + 1]) /* For decreasing order use '<' instead of '>' */
            {
                //printf("Winners = %i > %i > %i\n", pairs[0].winner,  pairs[1].winner,  pairs[2].winner);
                //printf("Losers = %i > %i > %i\n", pairs[0].loser,  pairs[1].loser,  pairs[2].loser);

                swap = pairs[d];
                pairs[d] = pairs[d + 1];
                pairs[d + 1] = swap;

                swapy = strong[d];
                strong[d] = strong[d + 1];
                strong[d + 1] = swapy;

                // printf("Winners = %i > %i > %i\n", pairs[0].winner,  pairs[1].winner,  pairs[2].winner);
                // printf("Losers = %i > %i > %i\n", pairs[0].loser,  pairs[1].loser,  pairs[2].loser);
                // printf("Strong = %i> %i> %i\n\n", strong[0], strong[1], strong[2]);
            }
        }
    }
}

bool check(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }
    else if (winner == -2)
    {
        return false;
    }

    int w;
    bool check1 = false;

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner])
        {
            check1 = true;
            w = i;
        }
    }
    if (check1 == false)
    {
        w = -2;
    }
    return check(w, loser);
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!check(pairs[i].winner, pairs[i].loser)) // If there is no cycle
        {
            locked[pairs[i].winner][pairs[i].loser] = true; // Lock the pair
            //printf("locked[%i][%i] = %d\n", pairs[i].winner, pairs[i].loser, locked[pairs[i].winner][pairs[i].loser]);
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int iswinner = 0;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                iswinner++;
                if (iswinner == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
}

