#include <iostream>
#include <velox/core/PlanNode.h>
#include <velox/vector/FlatVector.h>
#include <gtest/gtest.h>

using namespace facebook;
using namespace testing;

void flatVectorDemo(velox::memory::MemoryPool *pool_)
{
    // auto values = velox::AlignedBuffer::allocate<int32_t>(size, pool_);
    // auto vectorPtr = velox::FlatVector<int32_t>(
    //     memoryPool.get(),
    //     velox::INTEGER(),
    //     velox::BufferPtr(nullptr),
    //     size, std::move(values),
    //     std::vector<velox::BufferPtr>());
    const size_t vectorSize = 10;

    auto vectorPtr = velox::BaseVector::create(velox::INTEGER(), vectorSize, pool_);
    std::cout << "Created Vector of type INTEGER" << std::endl;
    std::cout << vectorPtr->toString() << std::endl;
    auto mutableValues = vectorPtr->values()->asMutable<int32_t>();
    std::cout << "Adding values to vector" << std::endl;
    for (size_t i = 0; i < vectorSize; i++)
        mutableValues[i] = (2 * i);
    for (size_t i = 0; i < vectorSize; i++)
        std::cout << "Value[" << i << "] = " << vectorPtr->toString(static_cast<velox::vector_size_t>(i)) << std::endl;

    uint64_t *mutableNulls = vectorPtr->mutableRawNulls();
    for (size_t i = 0; i < vectorSize; i++)
    {
        if (i & 1)
        {
            std::cout << "Setting null at " << i << std::endl;
            velox::bits::setNull(mutableNulls, i);
        }
    }

    auto flatVector = vectorPtr->asFlatVector<int32_t>();
    for (size_t i = 0; i < vectorSize; i++)
    {
        if (i & 1)
        {
            EXPECT_TRUE(vectorPtr->isNullAt(i));
            EXPECT_TRUE(flatVector->isNullAt(i));
        }
        else
        {
            EXPECT_EQ(flatVector->valueAt(static_cast<velox::vector_size_t>(i)), 2 * i);
        }
    }
}

void rowVectorDemo(velox::memory::MemoryPool *pool_)
{
    const size_t size = 10;
    // RowVector is equivalent of Arrow RecordBatch
    auto vectorPtr = velox::RowVector::create(velox::ROW({{"c0", velox::INTEGER()}, {"c1", velox::BIGINT()}, {"c2", velox::BOOLEAN()}}), size, pool_);
    auto rowPtr = vectorPtr->asChecked<velox::RowVector>();
    std::cout << "Creating Row Vector" << std::endl;
    ASSERT_NE(vectorPtr, nullptr);
    std::cout << rowPtr->children().size() << std::endl;
    std::cout << rowPtr->toString() << std::endl;
    auto c0 = rowPtr->childAt(0)->asFlatVector<int32_t>();
    auto c1 = rowPtr->childAt(1)->asFlatVector<int64_t>();
    auto c2 = rowPtr->childAt(2)->asFlatVector<bool>();
    ASSERT_NE(c0, nullptr);
    ASSERT_NE(c1, nullptr);
    ASSERT_NE(c2, nullptr);
    for (size_t i = 0; i < size; i++)
    {
        c0->set(static_cast<velox::vector_size_t>(i), i);
        c1->set(static_cast<velox::vector_size_t>(i), i * 2);
        c2->set(static_cast<velox::vector_size_t>(i), i % 2 == 0);
    }
    for (size_t i = 0; i < size; i++)
        std::cout << rowPtr->toString(static_cast<velox::vector_size_t>(i)) << std::endl;
    std::cout << "Setting nulls" << std::endl;
    auto nulls = vectorPtr->mutableRawNulls();
    for (size_t i = 0; i < size; i++)
    {
        if (i & 1)
        {
            velox::bits::setNull(nulls, i);
        }
    }

    std::cout << rowPtr->toString() << std::endl;

    rowPtr->setNullCount(5);
    ASSERT_EQ(rowPtr->getNullCount(), 5);
    for (size_t i = 0; i < size; i++)
        std::cout << rowPtr->toString(static_cast<velox::vector_size_t>(i)) << std::endl;
}

int main()
{
    // Init and Create Memory Manager
    auto opts = velox::memory::MemoryManager::Options{};
    auto memoryManager = new velox::memory::MemoryManager(opts);

    flatVectorDemo(memoryManager->addLeafPool("leaf<0>").get());
    rowVectorDemo(memoryManager->addLeafPool("lead<1>").get());
    return 0;
}