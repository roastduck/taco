#include "taco/type.h"

#include "error/error_messages.h"
#include "taco/util/collections.h"
#include "taco/util/strings.h"

#include <ostream>
#include <set>
#include <complex>

using namespace std;

namespace taco {

DataType::DataType() : kind(Undefined) {
}

DataType::DataType(Kind kind) : kind(kind) {
}

DataType::Kind DataType::getKind() const {
  return this->kind;
}

bool DataType::isUInt() const {
  return getKind() == UInt8 || getKind() == UInt16 || getKind() == UInt32 || getKind() == UInt64;
}

bool DataType::isInt() const {
  return getKind() == Int8 || getKind() == Int16 || getKind() == Int32 || getKind() == Int64;
}

bool DataType::isFloat() const {
  return getKind() == Float32 || getKind() == Float64;
}

bool DataType::isComplex() const {
  return getKind() == Complex64 || getKind() == Complex128;
}
  
size_t DataType::getNumBytes() const {
  return (getNumBits() + 7) / 8;
}

size_t DataType::getNumBits() const {
  switch (getKind()) {
    case UInt8:
    case Int8:
      return 8;
    case UInt16:
    case Int16:
      return 16;
    case UInt32:
    case Int32:
    case Float32:
      return 32;
    case UInt64:
    case Int64:
    case Float64:
    case Complex64:
      return 64;
    case Complex128:
      return 128;
    default:
      taco_ierror << "Bits for data type not set: " << getKind();
      return -1;
  }
}

std::ostream& operator<<(std::ostream& os, const DataType& type) {
  if (type.isInt()) os << "int" << type.getNumBits() << "_t";
  else if (type.isUInt()) os << "uint" << type.getNumBits() << "_t";
  else if (type == DataType::Float32) os << "float";
  else if (type == DataType::Float64) os << "double";
  else if (type == DataType::Complex64) os << "std::complex<float>";
  else if (type == DataType::Complex128) os << "std::complex<double>";
  else os << "Undefined";
  return os;
}

std::ostream& operator<<(std::ostream& os, const DataType::Kind& kind) {
  switch (kind) {
    case DataType::UInt8: os << "UInt8"; break;
    case DataType::UInt16: os << "UInt16"; break; 
    case DataType::UInt32: os << "UInt32"; break;
    case DataType::UInt64: os << "UInt64"; break;
    case DataType::Int8: os << "Int8"; break;
    case DataType::Int16: os << "Int16"; break;
    case DataType::Int32: os << "Int32"; break;
    case DataType::Int64: os << "Int64"; break;
    case DataType::Float32: os << "Float32"; break;
    case DataType::Float64: os << "Float64"; break;
    case DataType::Complex64: os << "Complex64"; break;
    case DataType::Complex128: os << "Complex128"; break;
    case DataType::Undefined: os << "Undefined"; break;
  }
  return os;
}

bool operator==(const DataType& a, const DataType& b) {
  return a.getKind() == b.getKind();
}

bool operator!=(const DataType& a, const DataType& b) {
  return a.getKind() != b.getKind();
}
  
DataType UInt8() {
  return DataType(DataType::UInt8);
}

DataType UInt16() {
  return DataType(DataType::UInt16);
}

DataType UInt32() {
  return DataType(DataType::UInt32);
} 

DataType UInt64() {
  return DataType(DataType::UInt64);
}

DataType Int8() {
  return DataType(DataType::Int8);
}

DataType Int16() {
  return DataType(DataType::Int16);
}

DataType Int32() {
  return DataType(DataType::Int32);
}

DataType Int64() {
  return DataType(DataType::Int64);
}

DataType Float32() {
  return DataType(DataType::Float32);
}

DataType Float64() {
  return DataType(DataType::Float64);
}

DataType Complex64() {
  return DataType(DataType::Complex64);
}

DataType Complex128() {
  return DataType(DataType::Complex128);
}

// class Dimension
Dimension::Dimension() : size(0) {
}

Dimension::Dimension(size_t size) : size(size) {
  taco_iassert(size > 0) << "Cannot create a dimension of size 0";
}

bool Dimension::isVariable() const {
  return getSize() == 0;
}

bool Dimension::isFixed() const {
  return getSize() > 0;
}

size_t Dimension::getSize() const {
  return size;
}

std::ostream& operator<<(std::ostream& os, const Dimension& dim) {
  return os << (dim.isFixed() ? util::toString(dim.getSize()) : "dynamic");
}


// class Shape
Shape::Shape(initializer_list<Dimension> dimensions) : dimensions(dimensions) {
}

Shape::Shape(std::vector<Dimension> dimensions)  : dimensions(dimensions) {
}

size_t Shape::numDimensions() const {
  return dimensions.size();
}

Dimension Shape::getDimension(size_t i) const {
  return dimensions[i];
}

std::vector<Dimension>::const_iterator Shape::begin() const {
  return dimensions.begin();
}

std::vector<Dimension>::const_iterator Shape::end() const {
  return dimensions.end();
}

std::ostream& operator<<(std::ostream& os, const Shape& shape) {
  return os << "[" << util::join(shape) << "]";
}


// class TensorType
Type::Type(DataType dtype, Shape shape)
    : dtype(dtype), shape(shape) {
}

DataType Type::getDataType() const {
  return dtype;
}

Shape Type::getShape() const {
  return shape;
}

std::ostream& operator<<(std::ostream& os, const Type& type) {
  return os << type.getDataType() << type.getShape();
}

}
