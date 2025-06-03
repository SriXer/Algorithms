#include "list.h"
#include <cstddef>

struct List;

struct ListItem {
    Data data;
    ListItem *prev;
    ListItem *next;
    List *parent;
};

struct List {
    ListItem *sentinel;
};

List *list_create() {
    List *list = new List;
    list->sentinel = new ListItem;
    list->sentinel->prev = list->sentinel;
    list->sentinel->next = list->sentinel;
    list->sentinel->parent = list;
    return list;
}

void list_delete(List *list) {
    ListItem *cur = list->sentinel->next;
    while (cur != list->sentinel) {
        ListItem *tmp = cur->next;
        delete cur;
        cur = tmp;
    }

    delete list->sentinel;
    delete list;
}

ListItem *list_first(List *list) {
    ListItem *first = list->sentinel->next;
    return (first == list->sentinel ? nullptr : first);
}

Data list_item_data(const ListItem *item) {
    return item->data;
}


//Убрал лишнюю проверку на parent, которая не давала корректно обойти список в некоторых случаях 
ListItem *list_item_next(const ListItem *item) {
    return (item->next != item->parent->sentinel ? item->next : nullptr);
}

ListItem *list_item_prev(const ListItem *item) {
    return (item->prev != item->parent->sentinel ? item->prev : nullptr);
}

//Добавил проверку на nullptr
ListItem *list_insert_after(List *list, ListItem *item, Data data) {
    ListItem *pos = item ? item : list->sentinel;
    if (item && item->parent != list)
        return nullptr;

    ListItem *node = new ListItem{ data, pos, pos->next, list };
    pos->next->prev = node;
    pos->next = node;
    return node;
}

ListItem *list_insert(List *list, Data data) {
    return list_insert_after(list, nullptr, data);
}

ListItem *list_erase_first(List *list) {
    if (list->sentinel->next == list->sentinel) return nullptr;
    return list_erase_next(list, nullptr);
}


//Добавил to_del->prev/next, указатели могли использоваться случайно где-то ещё
ListItem *list_erase_next(List *list, ListItem *item) {
    ListItem *pos = item ? item : list->sentinel;
    ListItem *to_del = pos->next;
    if (to_del == list->sentinel) return nullptr;

    ListItem *next = to_del->next;
    pos->next = next;
    next->prev = pos;

    to_del->prev = nullptr;
    to_del->next = nullptr;
    delete to_del;

    return (next == list->sentinel ? nullptr : next);
}
