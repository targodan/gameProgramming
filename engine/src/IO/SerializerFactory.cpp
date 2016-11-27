#include "SerializerFactory.h"

std::unique_ptr<engine::IO::SerializerFactory::BinarySerializer> engine::IO::SerializerFactory::binSerializer = nullptr;
std::unique_ptr<engine::IO::SerializerFactory::HumanReadableSerializer> engine::IO::SerializerFactory::humanSerializer = nullptr;