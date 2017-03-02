#include <vector>

class NaiveBayes{
    struct _Node;
    struct _Root;
public:


private:
    struct _Node{
        float probability;
        const _Node* parent;
    };
    struct _Root{
        float probability;
        std::vector<_Node> nodes;
    } m_Root;
};


int main(){
    return 0;
}




