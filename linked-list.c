#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct __node {
  int value;
  struct __node *next;
} node_t;

void add_entry(node_t **head, int new_value) {
  node_t **indirect = head;

  node_t *new_node = malloc(sizeof(node_t));
  new_node->value = new_value;
  new_node->next = NULL;

  assert(new_node); // AA1
  while (*indirect)
    indirect = &(*indirect)->next;
  *indirect = new_node; // AA2
}

node_t *find_entry(node_t *head, int value) {
  node_t *current = head;
  for (; current && current->value != value; current = current->next)
    /* interate */;
  return current;
}

void remove_entry(node_t **indirect, node_t *entry) {
  // node_t **indirect = head;
  while ((*indirect) != entry) {
    indirect = &(*indirect)->next;
  }
  *indirect = entry->next;
  free(entry);
}

node_t *swap_pair(node_t *head) {
  for (node_t **node = &head; *node && (*node)->next;
       node = &(*node)->next->next /* BB1*/) {
    node_t *tmp = *node;
    *node = (*node)->next; // BB2
    tmp->next = (*node)->next;
    (*node)->next = tmp;
  }
  return head;
}
void *swap_pair_p2p(node_t **node) {
  for (; *node && (*node)->next; node = &(*node)->next->next /* BB1*/) {
    node_t *tmp = *node;
    *node = (*node)->next; // BB2
    tmp->next = (*node)->next;
    (*node)->next = tmp;
  }
}
node_t *swap_pair_recursive(node_t *node) {
  if (!node || !node->next)
    return node;
  else {
    node_t *tmp = node;
    node = node->next; // BB2
    tmp->next = swap_pair_recursive(node->next);
    node->next = tmp;
    return node;
  }
}
node_t *reverse(node_t *head) {
  node_t *cursor = NULL;
  while (head) {
    node_t *next = head->next;
    head->next = cursor;
    cursor = head; // CCC
    head = next;
  }
  return cursor;
}
void reverse_p2p(node_t **indirect) {
  node_t *cursor = NULL;
  while (*indirect) {
    node_t *next = (*indirect)->next;
    (*indirect)->next = cursor;
    cursor = *indirect; // CCC
    *indirect = next;
  }
  *indirect = cursor;
}
node_t *reverse_recursive(node_t *head) {

  node_t *tmp, *new_head;
  if (!head->next) {
    return head;
  } else {
    new_head = tmp = reverse_recursive(head->next);
    for (; tmp->next; tmp = tmp->next)
      ;
    tmp->next = head;
    head->next = NULL;
    return new_head;
  }
}
void print_list(node_t *head) {
  for (node_t *current = head; current; current = current->next)
    printf("%d ", current->value);
  printf("\n");
}
void shuffle(node_t **head) {
  srand(time(NULL));
  // Calculate the length of list
  int len = 0;
  node_t *tmp = *head;
  while (tmp) {
    len++;
    tmp = tmp->next;
  }
  //  New list head to store the result of shuffle
  node_t *new_head = NULL;

  for (; len > 0; len--) {
    int rn = rand() % len;
    tmp = *head;

    // Reach the previous element of removed element
    rn -= 1;
    node_t *rm;
    if (rn == -1) {
      rm = *head;
      *head = (*head)->next;
    } else {
      while (rn--)
        tmp = tmp->next;

      rm = tmp->next;
      tmp->next = tmp->next->next;
    }
    // if the new_list is not empty
    if (new_head) {
      rm->next = new_head;
      new_head = rm;
    } else {
      new_head = rm;
      rm->next = NULL;
    }
  }
  *head = new_head;
}

int main(int argc, char const *argv[]) {
  node_t *head = NULL;

  // print_list(head);

  add_entry(&head, 72);
  add_entry(&head, 101);
  add_entry(&head, 108);
  add_entry(&head, 109);
  add_entry(&head, 110);
  add_entry(&head, 111);

  print_list(head);

  node_t *entry = find_entry(head, 101);
  remove_entry(&head, entry);
  printf("Remove 101 from list\n");
  print_list(head);

  /* swap pair.
   *      * See https://leetcode.com/problems/swap-nodes-in-pairs/
   *           */
  printf("Swap pair in list\n");
  head = swap_pair(head);
  print_list(head);

  printf("Reverse list\n");
  head = reverse(head);
  print_list(head);

  printf("Shuffle list\n");
  shuffle(&head);
  print_list(head);
  return 0;
}