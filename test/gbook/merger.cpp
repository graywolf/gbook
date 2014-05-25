using namespace std;
using namespace gbook;

namespace gbook {
    class contactsMock {
    public:
        MOCK_METHOD1(add, void(user &));
        MOCK_METHOD1(update, void(user));
        MOCK_METHOD1(remove, void(string));
    };
}

TEST(GbookMerger, newAbook) {
}