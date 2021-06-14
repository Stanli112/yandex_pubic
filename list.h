#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>

template <typename Type>
class SingleLinkedList {
    // ���� ������
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value{};
        Node* next_node = nullptr;
    };

    // ������ ������ ������� ��������.
    // ���������� ��������� ��������� �� �������� ������������ ������
    // ValueType - ��������� � Type (��� Iterator) ���� � const Type (��� ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // ����� ������ ����������� �������������, ����� �� ������� ������
        // ��� ������ � ��������� ������� ���������
        friend class SingleLinkedList;

        // �������������� ����������� ��������� �� ��������� �� ���� ������
        explicit BasicIterator(Node* node)
            : node_(node) {
        }

    public:
        // ����������� ���� ���� �������� ����������� ���������� � ��������� ����� ���������

        // ��������� ��������� - forward iterator
        // (��������, ������� ������������ �������� ���������� � ������������ �������������)
        using iterator_category = std::forward_iterator_tag;
        // ��� ���������, �� ������� ������������ ��������
        using value_type = Type;
        // ���, ������������ ��� �������� �������� ����� �����������
        using difference_type = std::ptrdiff_t;
        // ��� ��������� �� ����������� ��������
        using pointer = ValueType*;
        // ��� ������ �� ����������� ��������
        using reference = ValueType&;

        BasicIterator() = default;

        // �������������� �����������/����������� �����������
        // ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
        // ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_(other.node_) {
        }

        // ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
        // ����������������� ������������ �����������, ���� ������� �������� = �
        // �������� ���������� ������������� ��� �� ���.
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // ��������, �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        // �������� ��������� ���������� (� ���� ������� ��������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // ��������, �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������ �� ������ ����
        // ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������.
        // ���������� ������� �������� ���������
        // ��������� ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        BasicIterator operator++(int) noexcept {
            auto this_copy(*this);
            ++(*this);
            return this_copy;
        }

        // �������� �������������. ���������� ������ �� ������� �������
        // ����� ����� ���������, � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        // �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������.
        // ����� ����� ���������, � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // ��������, ����������� ��������� ��������� ������
    using Iterator = BasicIterator<Type>;
    // ����������� ��������, ��������������� ������ ��� ������ � ��������� ������
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        Assign(values.begin(), values.end());  // ����� ������� ����������
    }

    ~SingleLinkedList() {
        Clear();
    }

    // ���������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    // ���������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator end() noexcept {
        return {};
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    // ��������� ������ ������������ ������ ������ cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    // ��������� ������ ������������ ������ ������ cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cend() const noexcept {
        return {};
    }

    // ���������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

    // ���������� ���������� ��������� � ������ �� ����� O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // ��������, ������ �� ������ �� ����� O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return GetSize() == 0;
    }

    // ��������� ������� value � ������ ������ �� ����� O(1)
    void PushFront(const Type& value) {
        // ���� �������� new �������� ����������, ��������� head_.next_node ��������� �������
        head_.next_node = new Node(value, head_.next_node);

        // ����������� ��� �� ����������� ����������
        ++size_;
    }

    /*
     * ��������� ������� value ����� ��������, �� ������� ��������� pos.
     * ���������� �������� �� ����������� �������
     * ���� ��� �������� �������� ����� ��������� ����������, ������ ��������� � ������� ���������
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        auto& prev_node = pos.node_;
        assert(prev_node);

        //Node* new_node = new Node(value, pos.node_->next_node);
        //pos.node_->next_node = new_node;
        //++size_;

        //return Iterator(new_node);
        // ���� �������� new �������� ����������, ��������� prev_node->next_node ��������� �������
        prev_node->next_node = new Node(value, prev_node->next_node);

        // ����������� ��� �� ����������� ����������
        ++size_;
        return Iterator{ prev_node->next_node };
    }

    void PopFront() noexcept {
        assert(!IsEmpty());


        Node* intermediate = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete intermediate;

        //delete head_.next_node;
        //// ������ "������" ������ ��������� �� ������� ������ ����
        //head_.next_node = head_.next_node->next_node;
        --size_;
    }

    /*
     * ������� �������, ��������� �� pos.
     * ���������� �������� �� �������, ��������� �� ��������
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());

        Node* intermediate = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete intermediate;
        --size_;
        return Iterator(pos.node_->next_node);

        Node* const node_to_erase = pos.node_->next_node;
        delete node_to_erase;
        --size_;

        Node* const node_after_erased = node_to_erase->next_node;
        pos.node_->next_node = node_after_erased;

        return Iterator{ node_after_erased };
    }

    // ������� ������ �� ����� O(N)
    void Clear() noexcept {
        // ������� �������� ������� � ������ ������, ���� ��� �� ����������
        while (head_.next_node != nullptr) {

            Node* intermediate = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete intermediate;
        }
        size_ = 0;
    }

    // ���������� ���������� ������� �� ����� O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

private:
    template <typename InputIterator>
    void Assign(InputIterator from, InputIterator to) {
        // ������ ��������� ������, � ������� ����� ��������� �������� �� ��������� [from, to)
        // ���� � �������� ���������� ����� ��������� ����������,
        // ��� ���������� ��������� ��� ������
        SingleLinkedList<Type> tmp;

        // �������� ����� ������������ ������� � ��������� �� ������ ����
        Node** node_ptr = &tmp.head_.next_node;
        while (from != to) {
            // ���������, ��� ������� ��������� - �������
            assert(*node_ptr == nullptr);

            // ������ ����� ���� � ���������� ��� ����� � ��������� �������� ����
            *node_ptr = new Node(*from, nullptr);
            ++tmp.size_;

            // ������ node_ptr ������ ����� ��������� �� ��������� ����
            node_ptr = &((*node_ptr)->next_node);

            // ��������� � ���������� �������� ���������
            ++from;
        }

        // ������, ����� tmp �������� ����� ��������� ��������� [from, to),
        // ����� ��������� ����� ������� �������� ������� � tmp
        swap(tmp);
        // ������ ������� ������ �������� ����� ��������� ��������� [from, to),
        // � tmp - ������� �������� �������� ������
    }

    // ��������� ����, ������������ ��� ������� "����� ������ ���������"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (&lhs == &rhs)  // ����������� ��������� ������ � �����
        || (lhs.GetSize() == rhs.GetSize()
            && std::equal(lhs.begin(), lhs.end(), rhs.begin()));  // ����� ������� ����������
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);  // ����� ������� ����������
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());  // ����� ������� ����������
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);  // ����� ������� ����������
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);  // ����� ������� ����������
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);  // ����� ������� ����������
}