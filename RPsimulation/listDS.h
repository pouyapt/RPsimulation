#include "STDLibraries.h"

#ifndef listDS_h
#define listDS_h

namespace core {

template <class data_type>
class list
{
private: //------------------------------------------------------------
    struct node
    {
        data_type data;
        node* next;
        node* previous;
    };
    node* head;
    node* tail;
    int count;
    node* current;
    unsigned current_i;
    node* savedA;
    unsigned savedA_index;
    node* savedB;
    unsigned savedB_index;
    bool save_mode;
    bool save_turn;
    int save_factor;
public: //------------------------------------------------------------
    list (int save_factor_input=6)
    {
        set();
        if (save_factor_input < 6)
            save_factor_input = 6;
        save_factor = save_factor_input;
    }
    ~list ()
    {
        clear();
    }
    list (const list & source)
    {
        if (source.count==0)
            return;
        count = 0;
        node* temp = source.head;
        push_back (temp->data);
        while (temp->next != NULL)
        {
            temp = temp->next;
            push_back (temp->data);
        }
    }
    void operator = (const list & source)
    {
        if (source.count==0)
            return;
        clear();
        node* temp = source.head;
        push_back (temp->data);
        while (temp->next != NULL)
        {
            temp = temp->next;
            push_back (temp->data);
        }
    }
    data_type& operator [] (unsigned index)
    {
        if (index>=count)
            index = count - 1;
        find_node (index);
        save_current_node ();
        return current->data;
    }
    void push_front (data_type new_data)
    {
        node* new_node = make_node (new_data);
        if (count==0)
            initialize (new_node);
        else
        {
            head->previous = new_node;
            new_node->next = head;
            head = head->previous;
            head->previous = NULL;
        }
        count++;
        switch_save_mode();
        update_saved_nodes_push (0);
    }
    void push_back (data_type new_data)
    {
        node* new_node = make_node (new_data);
        if (count==0)
            initialize (new_node);
        else
        {
            tail->next = new_node;
            new_node->previous = tail;
            tail = tail->next;
            tail->next = NULL;
        }
        count++;
        switch_save_mode ();
    }
    void push (data_type new_data, unsigned index)
    {
        if (index>count)
            return;
        if (count==0)
        {
            push_back (new_data);
            return;
        }
        if (index==0)
        {
            push_front (new_data);
            return;
        }
        if (index==count)
        {
            push_back (new_data);
            return;
        }
        find_node (index);
        node* new_node = make_node (new_data);
        new_node->next = current;
        new_node->previous = current->previous;
        current->previous = new_node;
        new_node->previous->next = new_node;
        count++;
        switch_save_mode ();
        update_saved_nodes_push (index);
    }
    data_type pop_front ()
    {
        if (count==0)
            return NULL;
        data_type data = head->data;
        if (count==1)
        {
            delete head;
            head = NULL;
            tail = NULL;
        }
        else
        {
            data = head->data;
            node* new_node = head;
            head = head->next;
            head->previous = NULL;
            delete new_node;
            new_node = NULL;
        }
        count--;
        switch_save_mode();
        update_save_nodes_pop (0);
        return data;
    }
    data_type pop_back ()
    {
        if (count == 0)
            return NULL;
        node* temp;
        data_type data;
        if (count == 1)
        {
            data = tail->data;
            delete tail;
            head = NULL;
            tail = NULL;
        }
        else
        {
            data = tail->data;
            temp = tail;
            tail = tail->previous;
            tail->next = NULL;
            delete temp;
        }
        count--;
        switch_save_mode();
        update_save_nodes_pop (count-2);
        return data;
    }
    data_type pop (unsigned index)
    {
        if (index >= count)
            return NULL;
        data_type data;
        if (index==0)
        {
            data = pop_front();
            return data;
        }
        if (index==count-1)
        {
            data = pop_back();
            return data;
        }
        find_node (index);
        save_current_node();
        update_save_nodes_pop (index);
        current->previous->next = current->next;
        current->next->previous = current->previous;
        data = current->data;
        count--;
        switch_save_mode();
        delete current;
        return data;
    }
    void move_front (unsigned start_index, unsigned end_index)
    {
        if (start_index >= count || start_index == 0 || end_index >= count || end_index < start_index)
            return;
        node* start;
        node* end;
        bypass_range (start, end, start_index, end_index);
        connect_range_front (start, end);
        save_mode = 0;
        switch_save_mode();
    }
    void move_back (unsigned start_index, unsigned end_index)
    {
        if (start_index >= count || end_index >= count-1 || end_index < start_index)
            return;
        node* start;
        node* end;
        bypass_range (start, end, start_index, end_index);
        connect_range_back (start, end);
        save_mode = 0;
        switch_save_mode();
    }
    void move (unsigned start_index, unsigned end_index, unsigned target_index)
    {
        if (start_index >= count || end_index >= count || target_index >= count || end_index < start_index || (target_index >= start_index && target_index <= end_index))
            return;
        if (target_index == 0)
        {
            move_front (start_index, end_index);
            return;
        }
        if (target_index == count-1)
        {
            move_back (start_index, end_index);
            return;
        }
        find_node (target_index);
        node* target = current;
        node* start;
        node* end;
        bypass_range (start, end, start_index, end_index);
        connect_range_middle (start, end, target);
        save_mode = 0;
        switch_save_mode();
    }
    void extract (unsigned start_index, unsigned end_index, list & target_list, char mode='b')
    {
        if (count == 0 || start_index >= count || end_index >= count || end_index < start_index)
            return;
        if (start_index == 0 && end_index == count-1)
        {
            target_list.head = head;
            target_list.tail = tail;
            set();
            target_list.count = target_list.count + end_index - start_index + 1;
            target_list.switch_save_mode();
            return;
        }
        node* start;
        node* end;
        bypass_range (start, end, start_index, end_index);
        count = count - (end_index - start_index + 1);
        if (target_list.empty())
            mode = 'o';
        if (mode == 'o')
        {
            target_list.clear();
            target_list.head = start;
            target_list.tail = end;
        }
        else if (mode == 'b')
        {
            target_list.tail->next = start;
            start->previous = target_list.tail;
            target_list.tail = end;
        }
        else if (mode == 'f')
        {
            target_list.head->previous = end;
            end->next = target_list.head;
            target_list.head = start;
        }
        target_list.count = target_list.count + end_index - start_index + 1;
        target_list.switch_save_mode();
        switch_save_mode();
    }
    void concatenate_front (list & existing_list)
    {
        if (existing_list.count==0)
            return;
        if (count == 0)
        {
            head = existing_list.head;
            tail = existing_list.tail;
        }
        else
        {
            head->previous = existing_list.tail;
            existing_list.tail->next = head;
            head = existing_list.head;
            head->previous = NULL;
        }
        count = count + existing_list.count;
        existing_list.set();
        save_mode = 0;
        switch_save_mode();
    }
    void concatenate_back (list & existing_list)
    {
        if (existing_list.count==0)
            return;
        if (count == 0)
        {
            head = existing_list.head;
            tail = existing_list.tail;
        }
        else
        {
            tail->next = existing_list.head;
            existing_list.head->previous = tail;
            tail = existing_list.tail;
            tail->next = NULL;
        }
        count = count + existing_list.count;
        existing_list.set();
        switch_save_mode();
    }
    void concatenate (list & new_list, unsigned index)
    {
        if (new_list.count==0)
            return;
        if (index==0)
            concatenate_front (new_list);
        else if (index>=count)
            concatenate_back (new_list);
        else
        {
            find_node (index-1);
            current->next->previous = new_list.tail;
            new_list.tail->next = current->next;
            current->next = new_list.head;
            new_list.head->previous = current;
            count = count + new_list.count;
            new_list.set();
            save_mode = 0;
            switch_save_mode();
        }
    }
    void exchange_data (unsigned index1, unsigned index2)
    {
        if (index1>=count || index2>=count)
            return;
        data_type data;
        find_node (index1);
        save_current_node ();
        node* current_b = current;
        find_node (index2);
        save_current_node ();
        data = current->data;
        current->data = current_b->data;
        current_b->data = data;
    }
    void clear ()
    {
        while (count != 0)
        {
            node* temp;
            temp = tail;
            tail = tail->previous;
            delete temp;
            count--;
        }
        set();
    }
    bool empty ()
    {
        if (count==0)
            return true;
        else
            return false;
    }
    int size ()
    {
        return count;
    }
private: //------------------------------------------------------------
    void set ()
    {
        head = NULL;
        tail = NULL;
        current = NULL;
        count = 0;
        save_mode = 0;
    }
    void initialize (node* & new_node)
    {
        new_node->previous = NULL;
        new_node->next = NULL;
        tail = new_node;
        head = new_node;
    }
    node* make_node (data_type new_data)
    {
        node* new_node;
        new_node = new node;
        new_node->data = new_data;
        return new_node;
    }
    void move_forward (unsigned index)
    {
        while (current_i < index)
        {
            current = current->next;
            current_i++;
        }
    }
    void move_backward (unsigned index)
    {
        while (current_i > index)
        {
            current = current->previous;
            current_i--;
        }
    }
    int start_index (unsigned index)
    {
        int distance[4];
        distance[0] = index; //head
        distance[1] = count - index - 1; //tail
        distance[2] = savedA_index - index;
        if (distance[2] < 0)
            distance[2] = distance[2] * (-1);
        distance[3] = savedB_index - index;
        if (distance[3] < 0)
            distance[3] = distance[3] * (-1);
        int selected_node = 0;
        int selected_distance = distance[0];
        for (int i=1; i<4; i++)
        {
            if (distance[i] < selected_distance)
            {
                selected_node = i;
                selected_distance = distance[i];
            }
            
        }
        return selected_node;
    }
    void find_node_basic (unsigned index)
    {
        if (index < (count-index))
        {
            current = head;
            current_i = 0;
            move_forward (index);
        }
        else
        {
            current = tail;
            current_i = count - 1;
            move_backward (index);
        }
    }
    void find_node (unsigned index)
    {
        if (save_mode == 0)
        {
            find_node_basic (index);
            return;
        }
        int selected = start_index (index);
        if (selected == 0)
        {
            current = head;
            current_i = 0;
            move_forward (index);
            return;
        }
        if (selected == 1)
        {
            current = tail;
            current_i = count - 1;
            move_backward (index);
            return;
        }
        if (selected == 2)
        {
            current = savedA;
            current_i = savedA_index;
        }
        else
        {
            current = savedB;
            current_i = savedB_index;
        }
        if (index > current_i)
            move_forward (index);
        else if (index < current_i)
            move_backward (index);
    }
    void bypass_range (node* & start, node* & end, unsigned start_index, unsigned end_index)
    {
        find_node (start_index);
        start = current;
        find_node (end_index);
        end = current;
        if (start == head)
        {
            head = end->next;
            head->previous = NULL;
            end->next = NULL;
            return;
        }
        if (end == tail)
        {
            tail = start->previous;
            tail->next = NULL;
            start->previous = NULL;
            return;
        }
        start->previous->next = end->next;
        end->next->previous = start->previous;
        start->previous = NULL;
        end->next = NULL;
    }
    void connect_range_front (node* & start, node* & end)
    {
        end->next = head;
        head->previous = end;
        head = start;
    }
    void connect_range_back (node* & start, node* & end)
    {
        tail->next = start;
        start->previous = tail;
        tail = end;
    }
    void connect_range_middle (node* & start, node* & end, node* & target)
    {
        target->previous->next = start;
        start->previous = target->previous;
        end->next = target;
        target->previous = end;
    }
    void set_save_mode ()
    {
        find_node (count/3);
        savedA = current;
        savedA_index = current_i;
        find_node (count*2/3);
        savedB = current;
        savedB_index = current_i;
        save_mode = 1;
        save_turn = 0;
    }
    void switch_save_mode ()
    {
        if (save_mode == 1 && count >= save_factor)
            return;
        if (save_mode == 0 && count < save_factor)
            return;
        if (save_mode == 1 && count < save_factor)
            save_mode = 0;
        if (save_mode == 0 && count >= save_factor)
            set_save_mode();
    }
    void save_current_node ()
    {
        if (save_mode == 0 || current_i < save_factor/3 || current_i > count-save_factor*2/3)
            return;
        if (save_turn == 0)
        {
            savedA = current;
            savedA_index = current_i;
        }
        else
        {
            savedB = current;
            savedB_index = current_i;
        }
        save_turn = (~save_turn) & 1;
    }
    void update_saved_nodes_push (unsigned index)
    {
        if (save_mode == 0)
            return;
        if (index <= savedA_index)
            savedA_index++;
        if (index <= savedB_index)
            savedB_index++;
    }
    void update_save_nodes_pop (unsigned index)
    {
        if (save_mode == 0)
            return;
        if (savedA_index > index)
        {
            if (savedA_index > save_factor*2/3)
                savedA_index--;
            else if (savedA_index <= save_factor*2/3)
                savedA = savedA->next;
        }
        else if (savedA_index < index && savedA_index >= count-2)
        {
            savedA = savedA->previous;
            savedA_index--;
        }
        else if (savedA_index == index)
        {
            if (savedA_index >= count-(save_factor/2))
            {
                savedA = savedA->previous;
                savedA_index--;
            }
            else
                savedA = savedA->next;
        }
        if (savedB_index > index)
        {
            if (savedB_index > save_factor*2/3)
                savedB_index--;
            else if (savedB_index <= save_factor*2/3)
                savedB = savedB->next;
        }
        else if (savedB_index < index && savedB_index >= count-2)
        {
            savedB = savedB->previous;
            savedB_index--;
        }
        else if (savedB_index == index)
        {
            if (savedB_index >= count-(save_factor/2))
            {
                savedB = savedB->previous;
                savedB_index--;
            }
            else
                savedB = savedB->next;
        }
    }
};


};

#endif /* listDS_h */

