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
bool cycle(int loser, int winner);
void print_winner(void);

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
    // assign candidate to ranks[] in position rank (j in loop before)
    for (int i = 0; i < candidate_count; i++)
    {
        //check if name valid
        if (strcmp(name, candidates[i]) == 0)
        {
            //assign i to ranks, i is candidate position in candidates[]
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //r same as ranks. ranks has names, r has candidates position in candidates[]
    //preferences +1 for every position of candidates
    for (int i = 0; i < candidate_count; i++) // i= first in rank r
    {
        for (int j = i + 1; j < candidate_count; j++) // others later in rank r
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //in Pairs[].winner and Pairs[].loser there's the candidate number in candidates[]
    //i and j are the candidates order number
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                //add to pair [i][j]
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //bubble sort
    int n = 0;
    while (n < pair_count)
    {
        for (int i = 0; i < pair_count - 1; i++)
        {
            //swap in decreasing order
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                int t[] = {pairs[i].winner, pairs[i].loser};
                pairs[i].winner = pairs[i + 1].winner;
                pairs[i].loser = pairs[i + 1].loser;
                pairs[i + 1].winner = t[0];
                pairs[i + 1].loser = t[1];
            }
        }
        n++;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

//check if cycle
//a is loser, check if winner
bool cycle(int loser, int winner)
{
    // if loser == winner is cycle, return true
    if (loser == winner)
    {
        return true;
    }
    //i, interested when loser now is winner before creating a cycle
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (cycle(i, winner))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    //winner is the one with no arrows pointing towards it, so in locked[][] all "0" pointing to it (column j)
    for (int j = 0; j < candidate_count; j++)
    {
        int arrows = 0; //increase when arrow pointing, for candidate j (if locked[i][j] == true, the candidate has arrow pointing to it)
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][j] == true)
            {
                arrows++;
            }
        }
        if (arrows == 0)
        {
            printf("%s\n", candidates[j]);
            return;
        }

    }
    return;
}