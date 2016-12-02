#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int m[10][10] =
{   0,    1,    1,    0,    1,    1,    1,    1,    0,    1,
    0,    1,    0,    0,    0,    0,    1,    1,    1,    1,
    0,    1,    0,    0,    1,    0,    1,    1,    1,    1,
    0,    0,    0,    1,    1,    0,    1,    1,    1,    1,
    0,    1,    1,    1,    0,    0,    1,    0,    0,    1,
    1,    1,    1,    1,    0,    1,    1,    0,    0,    1,
    1,    1,    0,    1,    0,    0,    0,    0,    0,    1,
    1,    1,    0,    0,    1,    1,    1,    1,    0,    1,
    0,    1,    1,    0,    1,    1,    1,    1,    0,    1,
    0,    1,    1,    1,    1,    1,    1,    1,    0,    0
};

#define MAXPATH 102400		//maximum possible pathes, good and bad.
#define GOODPATH 1024		//maximum good pathes.

typedef struct node_struct {	//structure for points.
    int x, y;
    char pre_dire;		//pre_note is in which direction, l = left, r = right, u = up, d = down;
} node_stru;
node_stru path[MAXPATH][100];	//assume that all possible pathes (good or bad) are less than 102400, and number of steps of each patch is smaller than 100;

typedef struct good_path_struct {	//structure for good path
    int path_idx;
    int step_num;
} good_path_stru;
good_path_stru good_path[GOODPATH+1];	//assume that number of good pathes is no more than 1024;

void copy_path(int src_path, int dst_path, int last_idx, node_stru fork);//copy an existing path to a new path, add a next point to the new path.
int get_next_step(node_stru cur_node, node_stru * next); //from current point, in how many directions can it go (<=3), save the next points in these directions in link 'next'.
int is_point_exist(int path_idx, int steps); //check if a point already exists in the current path.
void print_msg(int good_path_num);

int main ()
{
    int all_path = 1;	//for count number of all possible pathes.
    int cur_path = 0;	//index of current path.
    int steps;		//count steps of a path.
    int num_dir;	//number of possible next steps for a certain point.
    int good_path_num = 0;	//count number of good pathes.
    node_stru next_step[3];	//for saving possible next points.

    memset(path, -1, sizeof(node_stru)*MAXPATH*100);
    memset(good_path, -1, sizeof(good_path_stru)*(GOODPATH+1));
    path[0][0].x = 0; //set the start point to (0,0).
    path[0][0].y = 0;

    while (cur_path < all_path && all_path < MAXPATH)
    {
        steps = 0;
        while(path[cur_path][steps+1].x != -1 && steps+1 < 100) { //if this path is copied from a path, go straight to the last point.
            steps++;
        }

        while ((path[cur_path][steps].x != 9 || path[cur_path][steps].y != 9) && steps < 100) {
            //does this point already exist in the path.
            if (is_point_exist(cur_path,steps)) {//yes, a loop, bad path.
                cur_path++;
                break;
            }
            
            //get next steps.
            num_dir = 0;
            num_dir = get_next_step(path[cur_path][steps], next_step);//how many directions can go.
            if (num_dir <= 0) {// no further step, bad path.
                cur_path++;
                break;
            } else if (num_dir == 1) {//a straight way, go ahead;
                steps++;
                path[cur_path][steps] = next_step[0];
                continue;
            } else {// has braches, need copy path.
                int m = 1;
                for (; m < num_dir; m++) {
                    if (all_path < MAXPATH) {
                        copy_path(cur_path, all_path, steps, next_step[m]); //add a path, copy current path and the next point in one branch.
                        all_path++;
                    } else { //too many possible pathes. exit.
                        break;
                    }
                }
                steps++;
                path[cur_path][steps] = next_step[0]; //go ahead in another branch.
                continue;
            }
        }
        if (good_path_num >= GOODPATH) {
            printf("good pathes are too many!! try to increase the GOODPATH.\n");
            break;
        }
        if (path[cur_path][steps].x == 9 && path[cur_path][steps].y == 9 && good_path_num < GOODPATH) {// a good path, remember it.
            good_path[good_path_num].path_idx = cur_path;
            good_path[good_path_num].step_num = steps;
            good_path_num++;
            cur_path++;
        }
    }
    if (all_path >= MAXPATH) {
        printf("Possible pathes are too many!! try to increase the MAXPATH.\n");
    }
    print_msg(good_path_num);
    return 0;
}

void copy_path(int src_path, int dst_path, int last_idx, node_stru fork)
{
    memcpy(path[dst_path],path[src_path],sizeof(node_stru)*(last_idx+1));
    path[dst_path][last_idx+1] = fork;
}

int get_next_step(node_stru cur_node, node_stru * next)
{
    int num_fork = 0;
    if (cur_node.x > 0) {
        if (m[cur_node.x-1][cur_node.y] == 0 && cur_node.pre_dire != 'l') {
            next[num_fork].x = cur_node.x-1;
            next[num_fork].y = cur_node.y;
            next[num_fork].pre_dire = 'r';
            num_fork++;
        }
    }
    if (cur_node.x < 9) {
        if (m[cur_node.x+1][cur_node.y] == 0 && cur_node.pre_dire != 'r') {
            next[num_fork].x = cur_node.x+1;
            next[num_fork].y = cur_node.y;
            next[num_fork].pre_dire = 'l';
            num_fork++;
        }
    }
    if (cur_node.y > 0) {
        if (m[cur_node.x][cur_node.y-1] == 0 && cur_node.pre_dire != 'u') {
            next[num_fork].x = cur_node.x;
            next[num_fork].y = cur_node.y-1;
            next[num_fork].pre_dire = 'd';
            num_fork++;
        }
    }
    if (cur_node.y < 9) {
        if (m[cur_node.x][cur_node.y+1] == 0 && cur_node.pre_dire != 'd') {
            next[num_fork].x = cur_node.x;
            next[num_fork].y = cur_node.y+1;
            next[num_fork].pre_dire = 'u';
            num_fork++;
        }
    }
    return num_fork;
}

int is_point_exist(int path_idx, int steps) //1 for exist, 0 for not exist.
{
    int i = 0;
    for (; i < steps-1; i++) {
        if (path[path_idx][i].x == path[path_idx][steps-1].x && \
            path[path_idx][i].y == path[path_idx][steps-1].y) // already exist.
        {
            return 1;
        }
    }
    return 0;
}

void print_path(int idx)
{
    int i = 0;
    for (; path[idx][i].x != -1; i++) {
        printf("\t (%d, %d)",path[idx][i].x,path[idx][i].y);
    }
    printf("\n");
}

void print_msg(int good_path_num)
{
    if (good_path_num <= 0) {// no good path found.
        printf("No path found!\n");
        return;
    } else {
        printf("found %d good pathes\n",good_path_num);
    }
    //now compare all good pathes, get the shortest path.
    int i = 0;
    int num_steps = 200;
    int shortest_path_idx = -1;
    for (; i < good_path_num; i++) {
        if (num_steps > good_path[i].step_num) {
            num_steps = good_path[i].step_num;
            shortest_path_idx = good_path[i].path_idx;
        }
    }
    printf("Shortest path is found, %d steps.\n",num_steps);
    int q = 0;
    for (i=0; i < good_path_num; i++) {
        if (good_path[i].step_num == num_steps) {
            if (q != 0) {
                printf("\nor\n");
            }
            print_path(good_path[i].path_idx);
            q++;
        }
    }
}
