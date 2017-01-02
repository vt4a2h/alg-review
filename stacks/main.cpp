#include <iostream>
#include <stack>
#include <vector>
#include <list>

#include <boost/range/algorithm_ext/erase.hpp>

template <class T>
class QStack : public std::stack<T>
{
  public:
    T take() {
        T v = this->top();
        this->pop();
        return v;
    }

    T takeBottom() {
        T v = this->c.front();
        this->c.pop_front();
        return v;
    }

    void print() const
    {
        for (auto && v : this->c)
            std::cout << v << "\t";
        std::cout << std::endl;
    }
};

// Stack of Plates. Composite stack behaves as usual stack
namespace SoP
{
    template <class T>
    class CompositeStack
    {
    public:
        void push(T const& t)
        {
            if (m_stacks.empty() || m_stacks.back().size() == capacity())
                m_stacks.emplace_back(QStack<T>());

            m_stacks.back().push(t);
        }

        T take()
        {
            if (empty())
                throw std::logic_error("The compoiste stack is empty.");

            auto &&back = m_stacks.back();
            T v = back.take();
            if (back.empty())
                m_stacks.erase(--std::end(m_stacks));
            return v;
        }

        T takeAt(std::size_t index)
        {
            if (index >= m_stacks.size())
                throw std::logic_error("Cannot take from this stack.");

            return leftShift(index, true /*removeTop*/);
        }

        bool empty() const { return m_stacks.empty(); }

        static std::size_t capacity() { return 3; }

    private:
        T leftShift(std::size_t index, bool removeTop)
        {
            auto &&stack = m_stacks.at(index);
            T removedItem = removeTop ? stack.take() : stack.takeBottom();
            if (stack.empty())
                boost::range::remove_erase(m_stacks, stack);
            else if (m_stacks.size() > index + 1)  {// not last element
                T v = leftShift(index + 1, false /*remove top*/);
                stack.push(v);
            }

            return removedItem;
        }

        std::vector<QStack<T>> m_stacks;
    };

    using IntCompositeStack = CompositeStack<int>;
}

// Implement Queue via two stacks
namespace tsq
{
    template <class T>
    class Queue
    {
    public:
        std::size_t size() const { return m_oldest.size() + m_newest.size(); }

        void add(T const& e)
        {
            // Newest stack always has new element on top
            m_newest.push(e);
        }


        T & peek() { return const_cast<T&>(const_cast<Queue<T> *>(this)->peek()); }
        T const& peek() const
        {
            shiftStacks();
            return m_oldest.top();
        }

        T take()
        {
            shiftStacks();
            return m_oldest.take();
        }

    private:
        /// Moves elements from newest stack to oldest
        void shiftStacks() const
        {
            if (m_oldest.empty())
                while (!m_newest.empty())
                    m_oldest.push(m_newest.take());
        }

        mutable QStack<T> m_oldest;
        mutable QStack<T> m_newest;
    };

    using IntQueue = Queue<int>;
}

// Implement sorted stack with using two stacks
namespace ss
{
    template <class T>
    class SortedStack
    {
    public:
        T const & peek() const { return m_stack.pop(); }
        T  & peek() { return m_stack.top(); }

        T take() { return m_stack.take(); }

        void push(T const& v)
        {
            m_stack.push(v);
            sort();
        }

        bool empty() const { return m_stack.empty(); }

    private:
        void sort()
        {
            QStack<T> tmpStask;
            while (!m_stack.empty()) {

                // Insert each element to the s in the sorted order
                int tmpVal = m_stack.take();
                while (!tmpStask.empty() && tmpStask.top() > tmpVal)
                    m_stack.push(tmpStask.take());

                tmpStask.push(tmpVal);

                std::cout << "stask: ";
                m_stack.print();
                std::cout << "tmp: ";
                tmpStask.print();
            }

            // Copy elements back
            while (!tmpStask.empty())
                m_stack.push(tmpStask.take());
        }

        QStack<T> m_stack;
    };

    using IntStack = SortedStack<int>;
}

// Animal shelter. Implement functions to "adopt" the oldest animal (in general) or perticulat animal
// (i.e. dog or cat).
namespace as
{
    struct Animal
    {
        std::string name;

        // Easy one, we don't have any behavioural difference between dogs and cats in this model
        enum Type { Dog, Cat };
        Type type;
    };

    class AnimalQueue
    {
    public:
        void enqueue(Animal const& animal)
        {
            if (animal.type == Animal::Dog)
                m_dogs.push_back({s_animalCounter++,animal});
            else
                m_cats.push_back({s_animalCounter++,animal});
        }

        Animal dequeueAny()
        {
            if (m_dogs.empty())
                return dequeueCat();

            if (m_cats.empty())
                return dequeueDog();

            OrderedAnimal dog = m_dogs.back();
            OrderedAnimal cat = m_cats.back();
            if (dog.first > cat.first) {
                m_dogs.pop_back();
                return dog.second;
            } else {
                m_cats.pop_back();
                return cat.second;
            }
        }

        Animal dequeueDog()
        {
            return dequeueImpl(m_dogs);
        }

        Animal dequeueCat()
        {
            return dequeueImpl(m_cats);
        }

    private:
        using OrderedAnimal = std::pair<std::size_t, Animal>;
        using AnimalsList = std::list<OrderedAnimal>;

        Animal dequeueImpl(AnimalsList & animals)
        {
            if (animals.empty())
                throw std::logic_error("No animals of this type or at all.");

            OrderedAnimal animal = animals.back();
            animals.pop_back();
            return animal.second;
        }

        static std::size_t s_animalCounter;

        AnimalsList m_dogs;
        AnimalsList m_cats;
    };

    std::size_t AnimalQueue::s_animalCounter = 0;

    void print(Animal const& animal)
    {
        std::cout << animal.name
                  << "\t"
                  << (animal.type == Animal::Dog ? "dog" : "cat") << std::endl;
    }
}

int main(int /*argc*/, char */*argv*/[])
{
    // 1
//    SoP::IntCompositeStack intStack;

//    for (std::size_t i = 0; i < 10; ++i)
//        intStack.push(i);

//    std::cout << intStack.takeAt(3) << "\n" << std::endl;

//    while (!intStack.empty())
//        std::cout << intStack.take() << std::endl;

    // 2
//    tsq::IntQueue queue;
//    for (int i = 0; i < 10; ++i)
//        queue.add(i);

//    while (queue.size() != 0)
//        std::cout << queue.take() << std::endl;

    // 3
//    ss::IntStack s;
//    s.push(10);
//    s.push(3);
//    s.push(12);
//    s.push(7);
//    s.push(5);

//    while (!s.empty())
//        std::cout << s.take() << std::endl;

    // 4
    as::AnimalQueue queue;
    queue.enqueue({"a1", as::Animal::Dog});
    queue.enqueue({"a2", as::Animal::Cat});
    queue.enqueue({"a3", as::Animal::Dog});
    queue.enqueue({"a4", as::Animal::Dog});
    queue.enqueue({"a5", as::Animal::Cat});

    try {
        as::print(queue.dequeueAny());
        as::print(queue.dequeueDog());
        as::print(queue.dequeueCat());
        as::print(queue.dequeueCat());
    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
