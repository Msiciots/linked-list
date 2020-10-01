# Practice on linked list


## 題目
### 測驗1
考慮一個單向 linked list，其結構定義為:
```c
typedef struct __node {                   
    int value;
    struct __node *next;
} node_t;
```
已知不存在 circular (環狀結構)，接下來將對該單向 linked list 進行下列操作:

* `add_entry`: 新增節點，當 linked list 沒有內容時，必須由開發者更新指向開頭的指標。因此實際得到 reference，而非 copy
* `remove_entry`: 移除指定節點，指向開頭的指標可能因此變更，所以需要用到 a pointer to a pointer (指標的指標)
* `swap_pair`: 交換一對相鄰的節點，取自 LeetCode: Swap Nodes in Pairs，給定 `1->2->3->4`，則該回傳 `2->1->4->3`
* `reverse`: 將給定的 linked list 其內節點予以反向，即 `1->2->3->4`，則該回傳 `4->3->2->1`
對應的程式碼如下:
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct __node {
    int value;
    struct __node *next;
} node_t;

void add_entry(node_t **head, int new_value)
{
    node_t **indirect = head;

    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = new_value;
    new_node->next = NULL;

    assert(new_node);   // AA1
    while (*indirect)
        indirect = &(*indirect)->next;
    *indirect = new_node;  //AA2
}

node_t *find_entry(node_t *head, int value)
{
    node_t *current = head;
    for (; current && current->value != value; current = current->next)
        /* interate */;
    return current;
}

void remove_entry(node_t **head, node_t *entry)
{
    node_t **indirect = head;

    while ((*indirect) != entry)
        indirect = &(*indirect)->next;

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

node_t *reverse(node_t *head)
{
    node_t *cursor = NULL;
    while (head) {
        node_t *next = head->next;
        head->next = cursor; cursor = head; // CCC
        head = next;
    }
    return cursor;
}

void print_list(node_t *head)
{
    for (node_t *current = head; current; current = current->next)
        printf("%d ", current->value);
    printf("\n");
}

int main(int argc, char const *argv[])
{
    node_t *head = NULL;

    print_list(head);

    add_entry(&head, 72);
    add_entry(&head, 101);
    add_entry(&head, 108);
    add_entry(&head, 109);
    add_entry(&head, 110);
    add_entry(&head, 111);

    print_list(head);

    node_t *entry = find_entry(head, 101);
    remove_entry(&head, entry);

    entry = find_entry(head, 111);
    remove_entry(&head, entry);

    print_list(head);

    /* swap pair.
     * See https://leetcode.com/problems/swap-nodes-in-pairs/
     */
    head = swap_pair(head);
    print_list(head);

    head = reverse(head);
    print_list(head);

    return 0;
}
```
參考執行輸出: (第 1 行是換行符號)
```cpp
72 101 108 109 110 111 
72 108 109 110 
108 72 110 109
109 110 72 108
```
請補完程式碼。

延伸問題:

1. 解釋上述程式運作原理，搭配 Graphviz，比照 Linked List 練習題 在 HackMD 筆記上視覺化展現;
2. 函式 swap_pair 和 reverse 對於指標的操作方式顯然異於 add_entry 及 remove_entry，需要額外做 head = ... 的更新，請用指標的指標來改寫，並避免回傳指標;
3. 以遞迴改寫上述的 reverse，注意，你可能因此需要建立新的函式，如 rev_recursive，隨後在 reverse 函式中呼叫 rev_recursive;
4. 針對 singly-linked list 的節點，實作 Fisher–Yates shuffle，你應該儘量降低記憶體的使用量;
:::
## 程式運作
### `add_entry`
```c
void add_entry(node_t **head, int new_value)
{
    node_t **indirect = head;

    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = new_value;
    new_node->next = NULL;

    assert(new_node);
    while (*indirect)
        indirect = &(*indirect)->next;

    *indirect = new_node;
}
```
- 新增一個串列節點在尾端
#### 舉例
![](https://github.com/Msiciots/linked-list/raw/master/img/add_entry1.png)
上圖為原始串列，欲新增一筆資料 7 到串列中。利用 pointer to pointer 將 `indirect` point to `head`，且新增 `new_node` 並分配記憶體空間。第 17 行 `assert(new_node);` 確保正確分配 `new_node` 記憶體空間。再來尋找串列尾端加入。函式執行流程如下圖所示。

![](https://github.com/Msiciots/linked-list/raw/master/img/add_entry2.gif)



### `find_entry`
```cpp=23
node_t *find_entry(node_t *head, int value)
{
    node_t *current = head;
    for (; current && current->value != value; current = current->next)
        /* interate */;
    return current;
}
```
給定串列開頭節點 `head` 與欲找的值，以 for-loop 依序比對串列中節點值，最後回傳該節點位址

### `remove_entry`

- 在串列中找到特定節點刪除
#### 舉例
以該段程式碼為例
```c
int main(int argc, char const *argv[]) {
  node_t *head = NULL;

  add_entry(&head, 72);
  add_entry(&head, 101);
  add_entry(&head, 108);

  print_list(head);

  node_t *entry = find_entry(head, 101);
  remove_entry(&head, entry);
  print_list(head);

  return 0;
}

```
下圖為未進行 `remove_entry()` 的串列

![](https://github.com/Msiciots/linked-list/raw/master/img/remove_entry1.png)
```c
void remove_entry(node_t **head, node_t *entry)
{
    node_t **indirect = head;

    while ((*indirect) != entry)
        indirect = &(*indirect)->next;

    *indirect = entry->next;
    free(entry);
}
```
```c
node_t **indirect = head;
``` 
函數內使用 pointer to pointer 只到原本陣列開頭

![](https://github.com/Msiciots/linked-list/raw/master/img/remove_entry2.png)

```c
while ((*indirect) != entry)
    indirect = &(*indirect)->next
```
透過 while 迴圈找尋欲刪除的 `entry` 節點，若 `(*indirect) != entry` ，更新 `indirect` 到下一個串列節點的 `next` 位址

![](https://github.com/Msiciots/linked-list/raw/master/img/remove_entry3.png)

```c
*indirect = entry->next;
free(entry);
```
更新指到 `entry` 的 `next` 到下一個節點再釋放 `entry` 記憶體空間

![](https://github.com/Msiciots/linked-list/raw/master/img/remove_entry4.png)

`void remove_entry(node_t **head, node_t *entry);` 有段小瑕疵，其實不需要宣告 `**indirect` 透過直接操作 `**head` 就能達到同樣效果，等於多浪費了 stack 記憶體空間 8 Bytes （在 64 位元的電腦架構下）

### `swap_pair`
交換串列中相鄰的節點，取自 [LeetCode: Swap Nodes in Pairs](https://leetcode.com/problems/swap-nodes-in-pairs/)，給定 1->2->3->4，則該回傳 2->1->4->3
#### 舉例
以下列程式碼為例
```c
int main(int argc, char const *argv[]) {
  node_t *head = NULL;
  
  add_entry(&head, 1);
  add_entry(&head, 2);
  add_entry(&head, 3);
  add_entry(&head, 4);
  print_list(head);

  head = swap_pair(head)
  print_list(head);

  return 0;
}

```
```c
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
```
下圖顯示完整的交換流程圖解
![](https://github.com/Msiciots/linked-list/raw/master/img/swap_pair1.gif)
#### `swap_pair` 無回傳值版
利用 pointer to pointer 將 `swap_pair` 改寫為不須回傳指標
```c
void *swap_pair(node_t **node) {
  for (; *node && (*node)->next;
       node = &(*node)->next->next /* BB1*/) {
    node_t *tmp = *node;
    *node = (*node)->next; // BB2
    tmp->next = (*node)->next;
    (*node)->next = tmp;
  }
}
```
##### for-loop step 1. 
`node_t *tmp = *node;`

![](https://github.com/Msiciots/linked-list/raw/master/img/swap_pair2.png)

`*node = (*node)->next;`

![](https://github.com/Msiciots/linked-list/raw/master/img/swap_pair3.png)

`tmp->next = (*node)->next;`

![](https://github.com/Msiciots/linked-list/raw/master/img/swap_pair4.png)

`(*node)->next = tmp;`

![](https://github.com/Msiciots/linked-list/raw/master/img/swap_pair5.png)

#### `swap_pair` 遞迴版
```c
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
```
將原本第 48 行 `node->next` 丟入 recursive，即可陸續兩兩一對交換完成
### `reverse`
將給定的串列其內節點予以反向，即 1->2->3，則該回傳 3->2->1
#### 舉例
以下列程式碼為例
```c
int main(int argc, char const *argv[]) {
  node_t *head = NULL;
  
  add_entry(&head, 1);
  add_entry(&head, 2);
  add_entry(&head, 3);
  print_list(head);

  head = reverse(head)
  print_list(head);

  return 0;
}

```
```c
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
```
可以發現進行串列 `reverse` 需要至少 3 個指標分別記錄前中後節點
下圖為完整的 `reverse` 交換流程圖解

![](https://github.com/Msiciots/linked-list/raw/master/img/reverse1.gif)

#### reverse 無回傳版
利用 pointer to pointer 將 `reverse` 改寫為不須回傳指標
```cpp=53
void *reverse(node_t **indirect) {
  node_t *cursor = NULL;
  while (*indirect) {
    node_t *next = (*indirect)->next;
    (*indirect)->next = cursor;
    cursor = *indirect; // CCC
    *indirect = next;
  }
  *indirect = cursor;
}
```
##### while-loop step 1. 
`node_t *next = (*indirect)->next;`

![](https://github.com/Msiciots/linked-list/raw/master/img/reverse2.png)

```
(*indirect)->next = cursor;
cursor = *indirect;
```

![](https://github.com/Msiciots/linked-list/raw/master/img/reverse3.png)

`*indirect = next;`

![](https://github.com/Msiciots/linked-list/raw/master/img/reverse4.png)

##### while-loop 結束
最後完成須更新 head

![](https://github.com/Msiciots/linked-list/raw/master/img/reverse5.png)

`*indirect = cursor;`

![](https://github.com/Msiciots/linked-list/raw/master/img/reverse6.png)

#### reverse 遞迴版

```c
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
```
- 改寫時先思考如何設立終止條件與重新串聯 list
- 因為原本是單向 linked list ，所以終止條件必定是最後一個 node
- 回傳 node 進行串接，注意保留最後一個 node 位址作為新的 `head`

## Fisher–Yates shuffle
參考 [Fisher–Yates shuffle](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle) 方法，把 Linked list 隨機排列
### Fisher and Yates' original method
Fisher–Yates shuffle 的原始版本，最初提出在 1938 年的 Ronald Fisher 和 Frank Yates 寫的書中，書名為《Statistical tables for biological, agricultural and medical research》。他們使用紙和筆去描述了這個演算法，並使用了一個隨機數表來提供隨機數。它給出了 1 到 N 的數字的的隨機排列，具體步驟如下：

1. 寫下從 1 到 N 的數字
2. 取一個從 1 到剩下的數字（包括這個數字）的隨機數 k
3. 從低位開始，得到第 k 個數字（這個數字還沒有被取出），把它寫在獨立的一個列表的最後一位
4. 重複第 2 步，直到所有的數字都被取出
5. 第 3 步寫出的這個序列，現在就是原始數字的隨機排列

已經證明如果第 2 步取出的數字是真隨機的，那麼最後得到的排序一定也是。
### The modern algorithm
Fisher–Yates shuffle 算法的現代版本是為電腦設計的。由 Richard Durstenfeld 在1964年提出。並且是被 Donald E. Knuth 在 《The Art of Computer Programming》 中推廣。但是不管是 Durstenfeld 還是 Knuth，都沒有在書的第一版中承認這個算法是 Fisher 和 Yates 的研究成果。也許他們並不知道。不過後來出版的 《The Art of Computer Programming》提到了 Fisher 和 Yates 貢獻。

現代版本的描述與原始略有不同，因為如果按照原始方法，電腦會花很多時間去計算上述第 3 步的剩餘數字。這裡的方法是在每次跌代時交換這個被取出的數字到原始列表的最後。這樣就將時間複雜度從 O(n^2) 減小到了 O(n)。pseudo code 如下：
```
-- To shuffle an array a of n elements (indices 0..n-1):
for i from n−1 downto 1 do
     j ← random integer such that 0 ≤ j ≤ i
     exchange a[j] and a[i]
```
下列為根據 The modern algorithm 的實作版本
```C
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
```
- 計算 Linked list 長度 `len` 用來得知迴圈執行次數
- 根據目前 `len` 產生隨機數，將隨機數 `rn - 1` 為了到達欲移除節點的前一節點
- 將原本 Linked list 移除節點
- 將移除節點連接到新 Linked list, `new_head`
- 迴圈結束，更新原本 `head` 為 `new_head`

