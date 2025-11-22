#include <iostream>
#include <velox/core/PlanNode.h>
#include <velox/vector/FlatVector.h>
#include <gtest/gtest.h>

using namespace facebook;
using namespace testing;

void flatVectorDemo(velox::memory::MemoryPool* pool_) {
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

int main()
{
    // Init and Create Memory Manager
    auto opts = velox::memory::MemoryManager::Options{};
    auto memoryManager = new velox::memory::MemoryManager(opts);

    flatVectorDemo(memoryManager->addLeafPool("leaf<0>").get());
    return 0;
}