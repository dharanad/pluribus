#include <iostream>
#include <velox/buffer/Buffer.h>
#include <velox/common/memory/Memory.h>

using namespace facebook;
int main() {
    std::cout << "Hello, Velox" << std::endl;
    // Create a memory manager
    velox::memory::MemoryManager::Options opts;
    velox::memory::MemoryManager memoryManager = velox::memory::MemoryManager(opts);
    // Create a memory pool
    auto pool_ = memoryManager.addLeafPool("main");
    // Create a alligned buffer
    velox::BufferPtr ptr = velox::AlignedBuffer::allocate<char>(10, pool_.get(), 'i');
    std::cout << ptr->size() << std::endl;
    return 0;    
}