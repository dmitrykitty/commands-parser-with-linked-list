#include <iterator>
#include <forward_list>
#include <deque>
#include <list>
#include <gtest/gtest.h>

#if __has_include("../objectlist.h")
    #include "../objectlist.h"
#elif __has_include("objectlist.h")
    #include "objectlist.h"
#else
    #error "File 'objectlist.h' not found!"
#endif
#include "../stringObject.h"

using namespace ::testing;


struct ObjectListTester : ::testing::Test
{
};

TEST_F(ObjectListTester, constructionWithDefaultConstructor_expectedAllElementsAreZero)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
    ASSERT_EQ(0, myList.size());
    ASSERT_TRUE(myList.empty());
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, pushFrontOfElements_expectedSizeIncreases)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_PUSH_FRONT
    ASSERT_EQ(0, myList.size());
    ASSERT_TRUE(myList.empty());

    constexpr size_t objects2Add = 10;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        myList.push_front(new StringObject{std::to_string(3.14 * i)});
        ASSERT_EQ(i, myList.size()) << "i=" << i;
        ASSERT_FALSE(myList.empty()) << "i=" << i;
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, pushFrontOfDuplicatedElements_expectedSizeIncreasesOnlyWhenUniqueElementAdded)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_PUSH_FRONT
    constexpr size_t objects2Add = 10;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        myList.push_front(new StringObject{"(" + std::to_string(i) + ")"});
        ASSERT_EQ(i, myList.size()) << "i=" << i;
        ASSERT_FALSE(myList.empty()) << "i=" << i;
    }

    // adding second time the same elements, but size does not change:
    for (size_t i=1; i <= objects2Add; ++i)
    {
        myList.push_front(new StringObject{"(" + std::to_string(i) + ")"});
        ASSERT_EQ(objects2Add, myList.size()) << "i=" << i;
    }

#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, popFrontOfElements_expectedSizeDecreases)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_POP_FRONT
    constexpr size_t objects2Add = 10;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        myList.push_front(new StringObject{std::to_string(3.14 * i)});
        ASSERT_EQ(i, myList.size()) << "i=" << i;
        ASSERT_FALSE(myList.empty()) << "i=" << i;
    }

    for (size_t i=1; i <= objects2Add; ++i)
    {
        myList.pop_front();
        ASSERT_EQ(objects2Add - i, myList.size()) << "i=" << i;
        const bool expectedEmpty = (i == objects2Add);
        ASSERT_EQ(expectedEmpty, myList.empty()) << "i=" << i;
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, frontOfLists_expectedCorrectFirstElementReturned)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_FRONT
    ASSERT_EQ(0, myList.size());
    ASSERT_TRUE(myList.empty());

    constexpr size_t objects2Add = 10;

    std::deque<StringObject*> objects;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        auto* object = new StringObject{">" + std::to_string(i) + "<"};
        myList.push_front(object);
        ASSERT_EQ(*object, myList.front());

        objects.push_front(object);
    }

    for (size_t i=1; i <= objects2Add; ++i)
    {
        ASSERT_EQ(*objects.front(), myList.front()) << "i=" << i;
        myList.pop_front();
        objects.pop_front();
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, clear_expectedAllElementsRemoved)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_CLEAR
    ASSERT_EQ(0, myList.size());
    ASSERT_TRUE(myList.empty());

    myList.clear();

    ASSERT_EQ(0, myList.size());
    ASSERT_TRUE(myList.empty());

    constexpr size_t objects2Add = 10;

    for (size_t i=1; i <= objects2Add; ++i)
    {
        myList.push_front(new StringObject{">" + std::to_string(i) + "<"});
    }

    ASSERT_EQ(objects2Add, myList.size());
    ASSERT_FALSE(myList.empty());

    myList.clear();

    ASSERT_EQ(0, myList.size());
    ASSERT_TRUE(myList.empty());
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, reverse_expectedAllElementsReversed)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_REVERSE
    constexpr size_t objects2Add = 10;

    std::deque<StringObject*> objects;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        auto* object = new StringObject{">" + std::to_string(i) + "<"};
        myList.push_front(object);

        objects.push_front(object);
    }
    myList.reverse();
    std::reverse(objects.begin(), objects.end());

    for (size_t i=1; i <= objects2Add; ++i)
    {
        ASSERT_EQ(*objects.front(), myList.front()) << "i=" << i;
        myList.pop_front();
        objects.pop_front();
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, iteratorMethodsSyntaxCorrect_expectedChecksPasses)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_ITERATOR_METHODS_AT_LEAST_EMPTY

    constexpr bool beginResultIsEmbeddedIterator = std::is_same_v<ObjectList::iterator, decltype(myList.begin())>;
    constexpr bool endResultIsEmbeddedIterator = std::is_same_v<ObjectList::iterator, decltype(myList.end())>;
    ASSERT_TRUE(beginResultIsEmbeddedIterator);
    ASSERT_TRUE(endResultIsEmbeddedIterator);

    constexpr bool iteratorDereferenceIsReturningReferenceToObject = std::is_same_v<Object&, decltype(*myList.begin())>;
    ASSERT_TRUE(iteratorDereferenceIsReturningReferenceToObject);

    constexpr bool iteratorIncrementationReturnsIterator = std::is_same_v<ObjectList::iterator, std::decay_t<decltype(++myList.begin())>>;
    ASSERT_TRUE(iteratorIncrementationReturnsIterator);

    constexpr bool comparingIteratorsReturnsBool = std::is_same_v<bool, decltype(myList.begin() != myList.end())>;
    ASSERT_TRUE(comparingIteratorsReturnsBool);
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, loopingWithIterators_expectedSuccess)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_ITERATORS
    constexpr size_t objects2Add = 10;

    std::deque<StringObject*> objects;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        auto* object = new StringObject{"->" + std::to_string(i) + "<-"};
        myList.push_front(object);

        objects.push_front(object);
    }

    auto itToStdDeque = objects.begin();

    for (auto it=myList.begin(); it != myList.end(); ++it, ++itToStdDeque)
    {
        ASSERT_EQ(**itToStdDeque, dynamic_cast<StringObject&>(*it));
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, eraseAfterMiddleElement_expectedMiddleElementErased)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_ERASE_AFTER
    constexpr size_t objects2Add = 10;

    std::forward_list<StringObject*> objects;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        auto* object = new StringObject{"->" + std::to_string(i) + "<-"};
        myList.push_front(object);

        objects.push_front(object);
    }

    auto it2MiddleElement = myList.begin();
    auto itToMiddleOfStdForwardList = std::next(objects.begin(), objects2Add/2);
    for (size_t i=0; i < objects2Add/2; ++i)
    {
        ++it2MiddleElement;
    }
    objects.erase_after(itToMiddleOfStdForwardList);
    myList.erase_after(it2MiddleElement);

    const auto sizeOfStdForwardList = std::distance(objects.begin(), objects.end());
    ASSERT_EQ(sizeOfStdForwardList, myList.size());

    auto itToStdForwardList = objects.begin();

    for (auto it=myList.begin(); it != myList.end(); ++it, ++itToStdForwardList)
    {
        ASSERT_EQ(**itToStdForwardList, dynamic_cast<StringObject&>(*it));
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, eraseMiddleElement_expectedMiddleElementErased)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_ERASE
    constexpr size_t objects2Add = 10;
    constexpr size_t element2EraseIndex = 2;

    std::list<StringObject*> objects;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        auto* object = new StringObject{"->" + std::to_string(i) + "<-"};
        myList.push_front(object);

        objects.push_front(object);
    }

    auto it2MiddleElement = myList.begin();
    auto itToMiddleOfStdDeque = std::next(objects.begin(), element2EraseIndex);
    for (size_t i=0; i < element2EraseIndex; ++i)
    {
        ++it2MiddleElement;
    }

    objects.erase(itToMiddleOfStdDeque);
    myList.erase(it2MiddleElement);

    ASSERT_EQ(objects.size(), myList.size());

    auto itToStdDeque = objects.begin();
    for (auto it=myList.begin(); it != myList.end(); ++it, ++itToStdDeque)
    {
        ASSERT_EQ(**itToStdDeque, dynamic_cast<StringObject&>(*it));
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, copyConstructorImplemented_expected2ListsWithTheSameElementsInTheSameOrder)
{
    ObjectList myList;

#ifndef UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
    constexpr size_t objects2Add = 10;

    std::list<StringObject*> objects;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        auto* object = new StringObject{"->" + std::to_string(i) + "<-"};
        myList.push_front(object);

        objects.push_front(object);
    }

    ObjectList myListCopy = myList;

    ASSERT_EQ(objects.size(), myListCopy.size());
    ASSERT_EQ(myList.size(), myListCopy.size());

    auto iteratorToStdList = objects.begin();
    for (auto it=myList.begin(), it2=myListCopy.begin();
         (it!=myList.end() && it2!=myListCopy.end());
         ++it, ++it2, ++iteratorToStdList)
    {
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it));
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it2));
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, moveConstructorImplemented_expectedAllElementsMovedToOrderContainer)
{
    ObjectList myList;

    constexpr size_t objects2Add = 10;

#ifndef UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
    std::list<StringObject*> objects;
    for (size_t i=1; i <= objects2Add; ++i)
    {
        auto* object = new StringObject{"->" + std::to_string(i) + "<-"};
        myList.push_front(object);

        objects.push_front(object);
    }

    ObjectList myListMoved = std::move(myList);

    ASSERT_EQ(objects.size(), myListMoved.size());
    ASSERT_EQ(0, myList.size());

    auto iteratorToStdList = objects.begin();
    for (auto it=myList.begin(); it!=myList.end(); ++it, ++iteratorToStdList)
    {
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it));
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, assignmentOperatorCopying_expectedNewValues)
{
    ObjectList myList1, myList2;

    constexpr size_t objects2Add1 = 10, objects2Add2 = 3;

#ifndef UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
    std::list<StringObject*> objects1, objects2;
    for (size_t i=1; i <= objects2Add1; ++i)
    {
        auto* object1 = new StringObject{"<" + std::to_string(i) + ">"};
        myList1.push_front(object1);
        objects1.push_front(object1);
    }

    for (size_t i=1; i <= objects2Add2; ++i)
    {
        auto* object2 = new StringObject{"[" + std::to_string(objects2Add2 - i) + "]"};
        myList2.push_front(object2);
        objects2.push_front(object2);
    }

    ASSERT_EQ(objects1.size(), myList1.size());
    ASSERT_EQ(objects2.size(), myList2.size());

    myList2 = myList1;
    ASSERT_EQ(objects1.size(), myList1.size());
    ASSERT_NE(objects2.size(), myList2.size());
    ASSERT_EQ(myList1.size(), myList2.size());

    auto iteratorToStdList = objects1.begin();
    for (auto it=myList1.begin(), it2=myList2.begin();
         (it!=myList1.end() && it2!=myList2.end());
         ++it, ++it2, ++iteratorToStdList)
    {
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it));
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it2));
    }

    //////////////// assignment to itselve:
    myList1 = myList1;
    iteratorToStdList = objects1.begin();
    for (auto it=myList1.begin(), it2=myList2.begin();
         (it!=myList1.end() && it2!=myList2.end());
         ++it, ++it2, ++iteratorToStdList)
    {
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it));
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it2));
    }

    //////////////// assignment to empty:
    ObjectList myList3;
    myList3 = myList1;
    iteratorToStdList = objects1.begin();
    for (auto it=myList1.begin(), it2=myList3.begin();
         (it!=myList1.end() && it2!=myList3.end());
         ++it, ++it2, ++iteratorToStdList)
    {
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it));
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it2));
    }

    //////////////// assignment empty to empty:
    ObjectList myList4, myList5;
    myList5 = myList4;

    ASSERT_EQ(0, myList5.size());
    ASSERT_EQ(myList4.size(), myList5.size());
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}

TEST_F(ObjectListTester, assignmentOperatorMoving_expectedAllElementsMoved)
{
    ObjectList myList1, myList2;

    constexpr size_t objects2Add1 = 10, objects2Add2 = 3;

#ifndef UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
    std::list<StringObject*> objects1, objects2;
    for (size_t i=1; i <= objects2Add1; ++i)
    {
        auto* object1 = new StringObject{"<" + std::to_string(i) + ">"};
        myList1.push_front(object1);
        objects1.push_front(object1);

        auto* object2 = new StringObject{"[" + std::to_string(objects2Add2 - i) + "]"};
        myList2.push_front(object2);
        objects2.push_front(object2);
    }

    ASSERT_EQ(objects1.size(), myList1.size());
    ASSERT_EQ(objects2.size(), myList2.size());

    myList1 = std::move(myList2);
    ASSERT_EQ(objects1.size(), myList1.size());
    ASSERT_EQ(0, myList2.size());

    auto iteratorToStdList = objects2.begin();
    for (auto it=myList1.begin(); it!=myList1.end(); ++it, ++iteratorToStdList)
    {
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it));
    }

    //////////////// assignment to itselve:
    myList1 = std::move(myList1);
    iteratorToStdList = objects2.begin();
    for (auto it=myList1.begin(); it!=myList1.end(); ++it, ++iteratorToStdList)
    {
        ASSERT_EQ(**iteratorToStdList, dynamic_cast<StringObject&>(*it));
    }
#else
    ADD_FAILURE() << "Not implemented!";
#endif
}
