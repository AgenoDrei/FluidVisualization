#include "VertexPositionNormal.h"

bool VertexPositionNormal::operator==(const VertexPositionNormal &other) const {
    return Position == other.Position && Normal == other.Normal;
}

bool VertexPositionNormal::operator!=(const VertexPositionNormal &other) const {
    return !(*this == other);
}

