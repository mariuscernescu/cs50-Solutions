#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        string namei = candidates[i].name;
        if (strcmp(namei, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;

}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // 1St Sort the array candidates[i] by candidates[i].votes
    candidate swap;

    for (int c = 0 ; c < candidate_count - 1; c++)
    {
        for (int d = 0 ; d < candidate_count - c - 1; d++)
        {
            if (candidates[d].votes < candidates[d + 1].votes) /* For decreasing order use '<' instead of '>' */
            {
                swap = candidates[d];
                candidates[d] = candidates[d + 1];
                candidates[d + 1] = swap;
            }
        }
    }
    // print winner(s)
    printf("%s\n", candidates[0].name);
    for (int i = 0; candidates[i].votes == candidates[i + 1].votes; i++) // If the first value in votes is equal with the second loop.
    {
        printf("%s\n", candidates[i + 1].name); // Print also the second name.
    }
}

