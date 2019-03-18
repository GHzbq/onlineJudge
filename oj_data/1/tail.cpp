
void test1()
{
    Solution s;
    bool ret = s.isnumber(121);
    if(ret)
    {
        std::cout << "Test1 ok!" << std::endl;
    }
    else
    {
        std::cout << "Test1 failed" << std::endl;
    }
}

void test2()
{
    Solution s;
    bool ret = s.isnumber(123);
    if(!ret)
    {
        std::cout << "Test2 ok!" << std::endl;
    }
    else
    {
        std::cout << "Test2 failed" << std::endl;
    }

}

// 测试代码
int main(void)
{
    test1();
    test2();
    return 0;
}
