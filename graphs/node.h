#pragma once
#pragma once

#include <memory>
#include <fstream>
#include <iostream>
#include <stdlib.h>

namespace Tree {

   /// Node class for tests, probably not optimal, but convenient
   template <class Key>
   struct Node : public std::enable_shared_from_this<Node<Key>>
   {
      enum Color {None, Red, Black};
      using Ptr  = std::shared_ptr<Node<Key>>;
      using WPtr = std::weak_ptr<Node<Key>>;

      Node() {}
      Node(const Ptr &p) : mParent(p) {}
      Node(Key k, Color c = None, const Ptr &p = nullptr) : mParent(p), mColor(c), mKey(k) {}

      Ptr makeLeftChild(Key k, Color c = None)  { return mLeftChild  = std::make_shared<Node<Key>>(k, c, ptr()); }
      Ptr makeRightChild(Key k, Color c = None) { return mRightChild = std::make_shared<Node<Key>>(k, c, ptr()); }

      Ptr parent() const { return mParent.lock(); }

      Ptr ptr()
      {
         try{
            return this->shared_from_this();
         } catch (...) {}

         return nullptr;
      }

      Ptr setLeftChild(Ptr const& l)
      {
          return setChild(mLeftChild, l);
      }

      Ptr setRightChild(Ptr const& r)
      {
          return setChild(mRightChild, r);
      }

      Ptr setChild(Ptr & child, Ptr const& n)
      {
          if (n)
              n->mParent = ptr();
          return child = n;
      }

      int depth() const
      {
          Ptr p = parent();
          int d = 0;
          while (p) {
              if (p.get() == this)
                  throw std::logic_error("There is a cycle. Cannot determine depth.");

              p = p->parent();
              ++d;
          }
          return d;
      }

      void dump(const std::string &path)
      {
         std::ofstream out(path, std::ofstream::out);
         out << "digraph {\n" << *this << "}\n";
         out.close();
      }

      friend std::ostream &operator <<(std::ostream &out, const Node<Key>& node)
      {
         streamColor(out, node.mKey, node.mColor);

         if (node.mLeftChild) {
            streamChildNode(out, node.mKey, *node.mLeftChild);
            out << *node.mLeftChild;
         }

         if (node.mRightChild) {
            streamChildNode(out, node.mKey, *node.mRightChild);
            out << *node.mRightChild;
         }

         return out;
      }

      WPtr mParent;
      Ptr  mLeftChild;
      Ptr  mRightChild;

      Color mColor = None;

      Key mKey = 0;
   };

   template <class Key>
   void streamColor(std::ostream &out, const Key& key, typename Node<Key>::Color color)
   {
      const bool isRed   = color == Node<Key>::Red;
      const bool isBlack = color == Node<Key>::Black;

      out << key
          << " [style=filled,color=black,fillcolor="
          << (isBlack || isRed ? isRed ? "red" : "black" : "white")
          << ",fontcolor="
          << (isBlack || isRed ? "white" : "black")
          << "];\n";
   }

   template <class Key>
   void streamChildNode(std::ostream &out, const Key& parentKey, const Node<Key>& node)
   {
      streamColor(out, node.mKey, node.mColor);
      out << parentKey << " -> " << node.mKey << ";\n";
   }

   using IntNode = Node<int>;
   using IntNodePtr = IntNode::Ptr;

} // namespace Tree

