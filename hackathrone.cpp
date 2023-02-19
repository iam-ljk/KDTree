#include <bits/stdc++.h>
#include <thread>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

class KDNode {
public:
    KDNode *left;
    KDNode *right;
    int index;
    vector<int> *point;
};

void median_with_rearrange(vector<vector<int>*> &points, int idx, int l, int r, int n) {
    int pivot = (*points[l])[idx];
    int i = l;
    int j = r;
    do {
        do{ i++; } while (i < r && (*points[i])[idx] <= pivot);
        do{ j--; } while (j >= l && (*points[j])[idx] > pivot);
        if(i < j) {
            vector<int>* temp = points[j];
            points[j] = points[i];
            points[i] = temp;
            //swap(points[i], points[j]);
        }
    } while(i < j);
    vector<int>* temp = points[j];
    points[j] = points[l];
    points[l] = temp;
    //swap(points[l], points[j]);

    if (j == n) {
        return;
    }
    if (j < n) {
        median_with_rearrange(points, idx, j+1, r, n);
    } else {
        median_with_rearrange(points, idx, l, j, n);
    }
}

class KDTree {
public:
    KDNode *root;
    KDTree(vector<vector<int>*> &points) {
        root = make_tree(0, points.size(), points.size(), 0, points);
        //print_tree(root);
    }

    void print_tree(KDNode *node) {
        if (node == NULL) return;
        print_tree(node->left);
        print_point(*node->point);
        print_tree(node->right);
    }

    void print_point(vector<int> &point) {
        cout << "(";
        for (int x: point) cout << x << ' ';
        cout << "), ";
    }

    void print_points(vector<vector<int>*> &points) {
        for (auto point: points) {
            cout << "(";
            for (int x: *point) cout << x << ' ';
            cout << "), ";
        }
        cout << endl;
    }

    KDNode* make_tree(int l, int r, int len, int level, vector<vector<int>*> &points) {
        //cout << l << ' ' << r << endl;
        if (r-l == 0) {
            return NULL;
        }
        int median = (r+l)/2;
        //cout << "Before: ";
        //print_points(points);
        median_with_rearrange(points, level, l, r, median);
        //cout << "After: ";
        //print_points(points);
        KDNode *kdnode = new KDNode();
        kdnode->point = points[median];
        kdnode->left = make_tree(l, median, median-l, (level+1)%(points[0]->size()), points);
        kdnode->right = make_tree(median+1, r, r - (median+1), (level+1)%(points[0]->size()), points);
        return kdnode;
    }

    int calcDist(vector<int> &point_a, vector<int> &point_b) {
        int sum = 0;
        for (int i = 0; i < (int)point_a.size(); i++) {
            sum += (point_b[i] - point_a[i])*(point_b[i] - point_a[i]);
        }
        return sum;
    }

     void _nearest(KDNode* node,vector<int> &point,int idx,KDNode *&best_node,int &best_dist) {
        if (node == NULL) {
            return; 
        }
        //cout << idx << ' ';
        //print_point(node->point);
        //cout << endl;
        KDNode* next_node;
        KDNode* other_node;
        if (point[idx] > (*node->point)[idx]) {
            next_node = node->right;
            other_node = node->left;
        } else {
            next_node = node->left;
            other_node = node->right;
        }

        _nearest(next_node, point, (idx+1)%point.size(), best_node, best_dist);

        //cout << "popping" << endl;
        int dist = calcDist(*(node->point), point);
        int dist2 = ((*node->point)[idx] - point[idx])*((*node->point)[idx] - point[idx]);
        if (dist < best_dist) {
            best_dist = dist;
            best_node = node;
        }

        //cout << dist2 << ' '  << best_dist << endl;
        if (dist2 < best_dist) {
            _nearest(other_node, point, (idx+1)%point.size(), best_node, best_dist);
        }
    }

    vector<int> nearest_index(vector<int> &point) {
        int best_dist = INT_MAX;
        KDNode *best_node = NULL;
        _nearest(root, point, 0, best_node, best_dist);
        return *best_node->point; 
    }
};
// void threadMethod(int low,int high,vector<vector<int>> test_point,KDTree tree) {
//   for(int i=low-1;i<=high;i++) {
//     struct timeval start_time, end_time;
//     long milli_time, seconds, useconds;
//     gettimeofday(&start_time, NULL);
//     vector<int> nearest_point = tree.nearest_index(test_point[i]);
//     gettimeofday(&end_time, NULL);
//     seconds = end_time.tv_sec - start_time.tv_sec; //seconds
//     useconds = end_time.tv_usec - start_time.tv_usec; //milliseconds
//     milli_time = ((seconds) * 1000 + useconds/1000.0);
//     cout << "Elapsed time: " << milli_time <<" milliseconds\n";
//   }
// }

int main() {
    cout << "Program Started!" << endl;
    vector<vector<int>*> points;
    for (int i = 0; i < 100000; i++) {
        vector<int> *p = new vector<int>(100);
        points.push_back(p);
        for (int j = 0; j < 100; j++) {
            (*points[i])[j] = rand()%1000;
        }
    }

    cout << "Input generated!" << endl;
    KDTree tree(points);
    cout << "Tree created!" << endl;

    vector<vector<int>> test_points;
    for (int q = 0; q < 100; q++) {
        vector<int> test_point(100);
        for (int i = 0; i < 100; i++) {
            test_point[i] = rand()%1000;
        }
        test_points.push_back(test_point);
    }
    // thread t1 (threadMethod,1,25,test_points,tree);
    // thread t2 (threadMethod,26,50,test_points,tree);
    // thread t3 (threadMethod,51,75,test_points,tree);
    // thread t4 (threadMethod,76,100,test_points,tree);
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    for(int q=0;q<100;q++) {
      struct timeval start_time, end_time;
      long milli_time, seconds, useconds;
      gettimeofday(&start_time, NULL);
      vector<int> nearest_point = tree.nearest_index(test_points[q]);
      gettimeofday(&end_time, NULL);
      seconds = end_time.tv_sec - start_time.tv_sec; //seconds
      useconds = end_time.tv_usec - start_time.tv_usec; //milliseconds
      milli_time = ((seconds) * 1000 + useconds/1000.0);
      cout << "Elapsed time: " << milli_time <<" milliseconds\n";
    }
    
    
    cout << "Done!" << endl;
}
