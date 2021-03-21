#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        auto new_node = new Node(e, nullptr);
        if (size_ == 0) {
            head_ = new_node;
            tail_ = new_node;

        } else {
            tail_->next = new_node;
            tail_ = new_node;
            tail_->next = nullptr;
        }
        size_++;
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);
        if (head_ == nullptr) {
            auto index_node = new Node(e, nullptr);
            head_ = index_node;
            tail_ = index_node;
        }
        if (index == size_) {
            auto index_node = new Node(e, nullptr);
            tail_->next = index_node;
            tail_ = index_node;
        }
        if (index == 0) {
            auto index_node = new Node(e, find_node(0));
            head_ = index_node;
        }
        if (index > 0 && index < size_) {
            auto index_node = new Node(e, find_node(index));
            find_node(index - 1)->next = index_node;
        }
        size_++;
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);
        find_node(index)->data = e;
    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        if (index == 0) {
            auto node = find_node(0);
            Element element = node->data;
            head_ = node->next;
            delete node;
            size_--;
            return element;
        }
        if (index > 0 && index < size_) {
            auto node = find_node(index);
            Element element = node->data;
            auto prev_node = find_node(index - 1);
            auto next_node = find_node(index + 1);
            prev_node->next = next_node;
            delete node;
            size_--;
            return element;
        }
        return {};
    }

    void LinkedList::Clear() {
        auto node_now = head_;
        while (node_now!=nullptr){
            auto next_node = node_now->next;
            delete node_now;
            node_now = next_node;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        assert(index >= 0 && index < size_);
        if (index == 0) {
            return head_->data;
        }
        if (index == size_ - 1) {
            return tail_->data;
        }

        int count = 0;
        for (Node *node = head_; node != nullptr; node = node->next) {
            if (count == index) return node->data;
            count++;
        }

        return {};
    }

    int LinkedList::IndexOf(Element e) const {
        int count = 0;
        for (Node *node = head_; node != nullptr; node = node->next) {
            if (node->data == e) return count;
            count++;
        }
        return -1;
    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);
        if (index == 0) {
            return head_;
        }

        if (index == size_ - 1) {
            return tail_;
        }

        int count = 0;
        for (Node *node = head_; node != nullptr; node = node->next) {
            if (count == index) return node;
            count++;
        }

        return {};
    }

// РЕАЛИЗОВАНО

    LinkedList::~LinkedList() {
        Clear();
    }

    bool LinkedList::Contains(Element e) const {
        // если индекс не найден, значит и элемента нет
        return IndexOf(e) == kNotFoundElementIndex;
    }

    int LinkedList::GetSize() const {
        return size_;
    }

    bool LinkedList::IsEmpty() const {
        return size_ == 0;
    }

    Element LinkedList::tail() const {
        // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
        return tail_ ? tail_->data : Element::UNINITIALIZED;
    }

    Element LinkedList::head() const {
        return head_ ? head_->data : Element::UNINITIALIZED;
    }

// === RESTRICTED AREA: необходимо для тестирования ===

    LinkedList::LinkedList(const std::vector<Element> &elements) {
        assert(!elements.empty());

        size_ = elements.size();
        head_ = new Node(elements[0], nullptr);

        auto current_node = head_;

        for (int index = 1; index < static_cast<int>(elements.size()); index++) {
            current_node->next = new Node(elements[index], nullptr);
            current_node = current_node->next;
        }
        tail_ = current_node;
    }

    std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
        if (list.head_ != nullptr && list.tail_ != nullptr) {
            os << "{ ";
            for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
                os << internal::elem_to_str(current_node->data) << ", ";
            }
            os << internal::elem_to_str(list.tail_->data) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
        if (list.size_ != static_cast<int>(elements.size())) return false;
        Node *current_node = list.head_;

        for (const auto e : elements) {
            if (current_node == nullptr) return false;
            if (current_node->data != e) return false;
            current_node = current_node->next;
        }
        return true;
    }

}  // namespace itis