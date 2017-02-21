#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <algorithm>
#include <glm/gtc/quaternion.hpp>

template <class T>
class VertexWelderResult {
public:
    std::vector<T> VertexBuffer;
    std::vector<int> IndexBuffer;
};

template <class T>
class VertexWelder {
public:
    typedef union {
        int i;
        float f;
    } u;

    static VertexWelderResult<T>* weld(std::vector<T>& input) {
        auto result = new VertexWelderResult<T>();
        std::map<unsigned __int128, int> hash;

        u a, b, c;

        auto currentIndex = 0;
        for(auto it = input.begin(); it != input.end(); it++) {
            a.f = it->Position.x;
            b.f = it->Position.y;
            c.f = it->Position.z;
            auto h  =(((unsigned __int128)a.i)<<64) + (((unsigned __int128)b.i)<<32) + ((unsigned __int128)c.i);

            auto element = hash.find(h);
            if(element == hash.end()) { // not found
                hash[h] = currentIndex;

                result->VertexBuffer.push_back(*it);
                result->IndexBuffer.push_back(currentIndex);
                currentIndex++;
            } else {
                result->VertexBuffer[element->second].Normal = glm::mix(result->VertexBuffer[element->second].Normal, it->Normal, 0.5f);
                result->IndexBuffer.push_back(element->second);
            }
        }

        return result;
    }

};



