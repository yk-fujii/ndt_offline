
#include <rosbag/bag.h>
#include <rosbag/view.h>

bool OpenRosBag(const std::string& bag_Name, rosbag::Bag& _bag, rosbag::View& _bagView,
                rosbag::View::iterator& _viewIterator)
{
  std::set<std::string> bagTopics;

  try
  {
    _bag.open(bag_Name, rosbag::bagmode::Read);

    rosbag::View view(_bag);
    std::vector<const rosbag::ConnectionInfo*> connection_infos = view.getConnections();
    std::vector<std::string> topics_to_subscribe;

    bagTopics.clear();

    BOOST_FOREACH (const rosbag::ConnectionInfo* info, connection_infos)
    {
      bagTopics.insert(info->topic);
    }

    if (bagTopics.find("/points_raw") == bagTopics.end())
    {
      std::cout << "Can't Find Points topic:" << std::endl;
      return false;
    }
    else
    {
      topics_to_subscribe.push_back(std::string("/points_raw"));
    }

    fprintf(stderr, "success open rosbag file\n");
    _bagView.addQuery(_bag, rosbag::TopicQuery(topics_to_subscribe));
    _viewIterator = _bagView.begin();
    return true;
  }
  catch (rosbag::BagIOException& e)
  {
    std::cout << "Can't Open Rosbag with path: " << bag_Name << std::endl;
    return false;
  }
}

int GetNextMsg(sensor_msgs::PointCloud2::ConstPtr& msg_ptr, rosbag::View& _bagView,
               rosbag::View::iterator& _viewIterator)
{
  if (_viewIterator != _bagView.end())
  {
    fprintf(stderr, "loading msg\n");
    rosbag::MessageInstance m = *_viewIterator;

    msg_ptr = m.instantiate<sensor_msgs::PointCloud2>();
    if (msg_ptr == 0)
    {
      return 0;
    }

    _viewIterator++;
    return 1;
  }
  else
  {
    return -1;
  }
}
