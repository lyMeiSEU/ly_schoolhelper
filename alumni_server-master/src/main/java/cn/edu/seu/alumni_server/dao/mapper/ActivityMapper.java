package cn.edu.seu.alumni_server.dao.mapper;


import cn.edu.seu.alumni_server.controller.dto.ActivityBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.SearchedActivityInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.StartedOrEnrolledActivityInfoDTO;
import cn.edu.seu.alumni_server.dao.entity.Activity;
import java.util.List;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

@Repository
public interface ActivityMapper extends Mapper<Activity> {

	/**
	 * 根据输入的活动 id 输出基本信息, 注意要输出基本
	 *
	 * @param activityId 活动 id.
	 * @return 查询的结果 json 对象.
	 */
	ActivityBasicInfoDTO getBasicInfosByActivityId(Long activityId);

	/**
	 * 根据输入的发起成员的 id, 获取到所有发起的活动的基本信息.
	 *
	 * @param accountId 发起成员 id.
	 * @return 该成员所有发起的活动.
	 */
	List<StartedOrEnrolledActivityInfoDTO> getBasicInfosByStartedAccountId(Long accountId);

	/**
	 * 获取该用户所有参与的活动的基本信息.
	 *
	 * @param accountId 参与活动的 id.
	 * @return list
	 */
	List<StartedOrEnrolledActivityInfoDTO> getBasicInfosByEnrolledAccountId(Long accountId);

	/**
	 * 向数据库模糊查询索要的活动的名字.
	 *
	 * @param nameKeyWord 活动名关键字.
	 * @return 被查到的所有的活动的信息.
	 */
	List<SearchedActivityInfoDTO> getActivitiesFuzzilyByActivityNameKeyWord(String nameKeyWord);

	/**
	 * 向数据库严格匹配查询活动名.
	 *
	 * @param activityName 活动名.
	 * @return 查询出的所有活动的基本信息.
	 */
	List<SearchedActivityInfoDTO> getActivitiesByActivityNameKeyWord(String activityName);

	// 判断是不是有一个
	Long hasAvailableActivity(Long activityId);

	// 逻辑删除一个活动
	void deleteActivityLogically(Long activityId);

	// 获取
	List<StartedOrEnrolledActivityInfoDTO> getActivitiesOfOneAlumniCircle(Long alumniCircleId);

	List<ActivityBasicInfoDTO> recommend(Long accountId);
}
