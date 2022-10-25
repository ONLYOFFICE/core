#pragma once

#include <memory>


namespace RedBlackTree
{
class IRBNode;
using PIRBNode = std::shared_ptr<IRBNode>;
using WPIRBNode = std::weak_ptr<IRBNode>;

enum Color
{
    RED = 0,
    BLACK = 1
};

class IRBNode : public std::enable_shared_from_this<IRBNode>
{
public:
    virtual void setLeft(PIRBNode pNode) = 0;
    virtual void setRight(PIRBNode pNode) = 0;
    virtual PIRBNode getLeft() const = 0;
    virtual PIRBNode getRight() const = 0;

    virtual void setColor(Color clr) = 0;
    virtual Color getColor()const = 0;

    virtual void setParent(PIRBNode pParent) = 0;
    virtual PIRBNode getParent() const = 0;

    virtual PIRBNode Grandparent() const = 0;
    virtual PIRBNode Sibling() const = 0;
    virtual PIRBNode Uncle() const = 0;

    virtual void AssignValueTo(PIRBNode other) = 0;

    virtual int CompareTo(const PIRBNode& other) const = 0;
    virtual std::wstring ToString() const = 0;
};
}
