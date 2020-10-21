#ifndef listDS_h
#define listDS_h

#include "parameters.h"

namespace core {

template <class data_type>
class list
{
private:
    struct node {
        data_type data;
        node* right;
        node* left;
    };
    node* first;
    node* last;
    long size_;
    node* current;
    long current_i;
    node* savedA;
    long savedA_i;
    node* savedB;
    long savedB_i;
    bool save_turn;
    int objectID;
    static int currentObject;
    static bool d_compare(data_type a, data_type b) {
        return (a<b ? true : false);
    }
    bool (*compare)(data_type, data_type);
public:
    list () {
        set();
        currentObject++;
        objectID = currentObject;
    }
    
    list (unsigned size__) {
        list();
        for (auto i=0; i<size__; i++)
        {
            add_node_back();
        }
    }
    
    ~list () {
        clear();
    }
    
    list (const list & source) {
        set();
        node* temp = source.first;
        for (auto i=0; i<source.size_; i++)
        {
            push_back(temp->data);
            temp = temp->right;
        }
    }
    
    list (list && other) {
        grab_and_add(other);
    }
    
    void clear () {
        while (size_ != 0)
        {
            node* temp = last;
            last = last->left;
            delete temp;
            size_--;
        }
    }

    auto size () {
        return size_;
    }

    bool empty () {
        if (size_ == 0)
            return true;
        else
            return false;
    }

    auto get_objectID() {
        return objectID;
    }
    
    data_type& begin () {
        return first->data;
    }
    
    data_type& end () {
        return last->data;
    }
    
    void push_back (data_type new_data) {
        if (size_ == 0)
            initialize();
        else
            add_node_back();
        last->data = new_data;
    }
    
    void push_front (data_type new_data) {
        if (size_ == 0)
            initialize();
        else
            add_node_front();
        first->data = new_data;
    }
    
    void push (long index, data_type new_data) {
        if (size_==0) {
            push_back(new_data);
            return;
        }
        adjust_index(index);
        if (index==0)
            push_front(new_data);
        else {
            find_node(index);
            node* new_node = new node;
            new_node->data = new_data;
            current->left->right = new_node;
            new_node->left = current->left;
            new_node->right = current;
            current->left = new_node;
            size_++;
            if (savedA_i >= current_i)
                savedA_i++;
            if (savedB_i >= current_i)
                savedB_i++;
            current_i++;
            save_current_node();
        }
    }
    
    data_type pop_back () {
        if (size_==0)
            return NULL;
        if (size_ == 1)
            return uninitialize();
        node* temp = last;
        last = last->left;
        last->right = first;
        first->left = last;
        size_--;
        if (temp==savedA) {
            savedA = last;
            savedA_i = size_ - 1;
        }
        if (temp==savedB) {
            savedB = last;
            savedB_i = size_ - 1;
        }
        current = last;
        current_i = size_ - 1;
        data_type data = temp->data;
        delete temp;
        return data;
    }
    
    data_type pop_front () {
        if (size_==0)
            return NULL;
        if (size_ == 1)
            return uninitialize();
        node* temp = first;
        first = first->right;
        first->left = last;
        last->right = first;
        if (temp == savedA)
            savedA = savedA->right;
        else
            savedA_i--;
        if (temp == savedB)
            savedB = savedB->right;
        else
            savedB_i--;
        data_type temp_data = temp->data;
        delete temp;
        size_--;
        current = first;
        current_i = 0;
        return temp_data;
    }
    
    data_type pop (long index) {
        if (size_==0)
            return NULL;
        if (size_ == 1)
            return uninitialize();
        adjust_index(index);
        if (index == 0)
            return pop_front();
        if (index == size_-1)
            return pop_back();
        find_node(index);
        current->left->right = current->right;
        current->right->left = current->left;
        if (index == savedA_i)
            savedA = savedA->right;
        else if (index < savedA_i)
            savedA_i--;
        if (index == savedB_i)
            savedB = savedB->right;
        else if (index < savedB_i)
            savedB_i--;
        node* temp_node = current;
        current = current->right;
        data_type temp = temp_node->data;
        delete temp_node;
        size_--;
        save_current_node();
        return temp;
    }
    
    void move_front (long start_index, long end_index) {
        adjust_index(start_index);
        adjust_index(end_index);
        if (start_index > end_index || start_index == 0)
            return;
        find_node(start_index);
        node* start = current;
        find_node(end_index);
        node* end = current;
        if (end == last)
            last = first->left;
        start->left->right = end->right;
        end->right->left = start->left;
        end->right = first;
        first->left = end;
        first = start;
        first->left = last;
        last->right = first;
        reset_saved();
    }
    
    void move_back (long start_index, long end_index) {
        adjust_index(start_index);
        adjust_index(end_index);
        if (start_index > end_index || end_index == size_-1)
            return;
        find_node(start_index);
        node* start = current;
        find_node(end_index);
        node* end = current;
        if (start == first)
            first = end->right;
        start->left->right = end->right;
        end->right->left = start->left;
        start->left = last;
        last->right = start;
        last = end;
        last->right = first;
        first->left = last;
        last->right = first;
        reset_saved();
    }
    
    void move (long start_index, long end_index, long target_index) {
        adjust_index(start_index);
        adjust_index(end_index);
        if (target_index >= start_index && target_index <= end_index+1)
            return;
        if (target_index==0)
            move_front(start_index, end_index);
        else if (target_index == size_)
            move_back(start_index, end_index);
        else
        {
            find_node(start_index);
            node* start = current;
            find_node(end_index);
            node* end = current;
            find_node(target_index);
            node* target = current;
            start->left->right = end->right;
            end->right->left = start->left;
            start->left = target->left;
            target->left->right = start;
            target->left = end;
            end->right = target;
            reset_saved();
        }
    }
    
    void extract (long start_index, long end_index, list & dest) {
        adjust_index(start_index);
        adjust_index(end_index);
        if (start_index > end_index)
            return;
        find_node(start_index);
        node* start = current;
        find_node(end_index);
        node* end = current;
        if (start==first)
            first = end->right;
        if (end==last)
            last = start->left;
        start->left->right = end->right;
        end->right->left = start->left;
        last->right = first;
        first->left = last;
        size_ = size_ - (end_index - start_index + 1);
        if (size_==0)
            set();
        else
            reset_saved();
        dest.clear();
        dest.first = start;
        dest.last = end;
        dest.last->right = dest.first;
        dest.first->left = dest.last;
        dest.size_ = end_index - start_index + 1;
        dest.reset_saved();
    }
    
    void grab_and_replace(list & target) {
        clear();
        first = target.first;
        last = target.last;
        size_ = target.size_;
        current = target.current;
        current_i = target.current_i;
        savedA = target.savedA;
        savedA_i = target.savedA_i;
        savedB = target.savedB;
        savedB_i = target.savedB_i;
        save_turn = target.save_turn;
        objectID = target.objectID;
        compare = target.compare;
        target.set();
    }
    
    void grab_and_add(list & target) {
        if (target.size_==0)
            return;
        if (size_==0) {
            grab_and_replace(target);
            return;
        }
        last->right = target.first;
        target.first->left = last;
        last = target.last;
        last->right = first;
        size_ += target.size_;
        target.set();
    }
    
    void sort(bool c_compare(data_type,data_type)=d_compare) {
        compare = c_compare;
        mergeSort(*this, 0, size_-1);
    }
    data_type& operator [] (long index) {
        adjust_index(index);
        find_node(index);
        save_current_node();
        return current->data;
    }
    
    void operator = (const list & source) {
        std::cout << "copied\n";
        clear();
        node* temp = source.first;
        for (long i=0; i<source.size_; i++)
        {
            push_back(temp->data);
            temp = temp->right;
        }
    }
    
    list<data_type> operator + (list & nextObj) {
        list<data_type> new_list = *this;
        for (auto i=0; i<nextObj.size(); i++)
        {
            new_list.push_back(nextObj[i]);
        }
        return new_list;
    }
    
    list<data_type> operator + (unsigned new_node) {
        for (auto i=0; i<new_node; i++)
        {
            this->add_node_back();
        }
        return *this;
    }
private:
    void set () {
        size_ = 0;
        first = NULL;
        last = NULL;
        savedA = NULL;
        savedB = NULL;
        save_turn = 0;
    }
    
    void reset_saved () {
        current = first;
        current_i = 0;
        savedA = first;
        savedB = first;
        savedA_i = 0;
        savedB_i = 0;
    }
    
    void initialize (data_type new_data) {
        node* new_node = new node;
        new_node->data = new_data;
        first = new_node;
        first->right = first;
        first->left = first;
        last = first;
        reset_saved();
        size_ = 1;
    }
    
    void initialize () {
        node* new_node = new node;
        first = new_node;
        first->right = first;
        first->left = first;
        last = first;
        reset_saved();
        size_ = 1;
    }
    
    data_type uninitialize () {
        size_ = 0;
        data_type temp = first->data;
        delete first;
        set();
        return temp;
    }
    
    void adjust_index (long & index) {
        index = index % size_;
        if (index < 0)
            index = index + size_;
    }
    
    void find_node (long & index) {
        long distanceA = index - savedA_i;
        if (abs(distanceA) > size_/2) {
            if (distanceA > 0)
                distanceA = abs(distanceA) - size_;
            else
                distanceA = size_ - abs(distanceA);
        }
        long distanceB = index - savedB_i;
        if (abs(distanceB) > size_/2) {
            if (distanceB > 0)
                distanceB = abs(distanceB) - size_;
            else
                distanceB = size_ - abs(distanceA);
        }
        if (abs(distanceA) <= abs(distanceB)) {
            current = savedA;
            current_i = savedA_i;
            move_to_node(distanceA);
        }
        else {
            current = savedB;
            current_i = savedB_i;
            move_to_node(distanceB);
        }
        adjust_index(current_i);
    }
    
    void move_to_node (long distance) {
        if (distance > 0) {
            while (distance) {
                current = current->right;
                current_i++;
                distance--;
            }
        }
        else if (distance < 0) {
            while (distance) {
                current = current->left;
                current_i--;
                distance++;
            }
        }
    }
    
    void save_current_node () {
        if (!save_turn)
        {
            savedA = current;
            savedA_i = current_i;
        }
        else {
            savedB = current;
            savedB_i = current_i;
        }
        save_turn = !save_turn;
    }
    
    void add_node_back() {
        if (size_ == 0)
            initialize ();
        else
        {
            node* new_node = new node;
            last->right = new_node;
            new_node->left = last;
            last = last->right;
            last->right = first;
            first->left = last;
            size_++;
            current = last;
            current_i = size_-1;
            save_current_node();
        }
    }
    
    void add_node_front() {
        if (size_ == 0)
            initialize ();
        else
        {
            node* new_node = new node;
            first->left = new_node;
            new_node->right = first;
            first = new_node;
            last->right = first;
            first->left = last;
            savedA_i++;
            savedB_i++;
            size_++;
            current = first;
            current_i = 0;
            save_current_node();
        }
    }
    
    void merge(list<data_type> &arr, long l, long m, long r) {
        long i, j, k;
        auto n1 = m - l + 1;
        auto n2 =  r - m;
        data_type *L = new data_type[n1];
        data_type *R = new data_type[n2];
        for (i = 0; i < n1; i++)
            L[i] = arr[l + i];
        for (j = 0; j < n2; j++)
            R[j] = arr[m + 1+ j];
        i = 0;
        j = 0;
        k = l;
        while (i < n1 && j < n2) {
            if (compare(L[i], R[j])) {
                arr[k] = L[i];
                i++;
            }
            else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
        delete [] L;
        delete [] R;
    }
    
    void mergeSort(list<data_type> &arr, long l, long r) {
        if (l < r) {
            long m = l+(r-l)/2;
            mergeSort(arr, l, m);
            mergeSort(arr, m+1, r);
            merge(arr, l, m, r);
        }
    }
    
};

template <class data_type>
int list<data_type>::currentObject = 0;

};

#endif /* listDS_h */

