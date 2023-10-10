#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"
#include <vector>
#include <algorithm>

class MergeArraysNode : public rclcpp::Node {
public:
    MergeArraysNode() : Node("merge_arrays_node") {
        subscription1 = this->create_subscription<std_msgs::msg::Int32MultiArray>(
            "/input/array1", 1, std::bind(&MergeArraysNode::array1Callback, this, std::placeholders::_1));

        subscription2 = this->create_subscription<std_msgs::msg::Int32MultiArray>(
            "/input/array2", 1, std::bind(&MergeArraysNode::array2Callback, this, std::placeholders::_1));

        publisher = this->create_publisher<std_msgs::msg::Int32MultiArray>("/output/array", 1);

        merged_array_.data.clear();
    }

    void array1Callback(const std_msgs::msg::Int32MultiArray::SharedPtr msg) {
        array1 = msg->data;
        mergeAndPublish();
    }

    void array2Callback(const std_msgs::msg::Int32MultiArray::SharedPtr msg) {
        array2 = msg->data;
        mergeAndPublish();
    }

    void mergeAndPublish() {

        std::vector<int> merged_vector;
        merged_vector.reserve(array1.size() + array2.size());
        std::merge(array1.begin(), array1.end(), array2.begin(), array2.end(), std::back_inserter(merged_vector));

        auto mergedMsg = std_msgs::msg::Int32MultiArray();
        mergedMsg.data = merged_vector;

        publisher->publish(mergedMsg);
    }
private:
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr subscription1_;
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr subscription2_;
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr publisher_;
    std::vector<int> array1;
    std::vector<int> array2;
    std_msgs::msg::Int32MultiArray merged_array_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MergeArraysNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}