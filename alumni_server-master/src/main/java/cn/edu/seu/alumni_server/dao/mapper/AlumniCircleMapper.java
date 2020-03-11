package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleMemberDTO;
import cn.edu.seu.alumni_server.dao.entity.AlumniCircle;

import java.util.List;

import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

@Repository
public interface AlumniCircleMapper extends Mapper<AlumniCircle> {

	/**
	 * 根据用户账号来查询所参加的校友圈信息.
	 *
	 * @param accountId 用户账号.
	 * @return 所有参与的校友圈信息.
	 */
	public List<AlumniCircleBasicInfoDTO> queryEnrolledAlumniCircleInfosByAccountId(Long accountId);

	/**
	 * 根据校友圈的名字来模糊查询.
	 *
	 * @param name 校友圈的名字.
	 * @return 模糊查询结果.
	 */
	public List<AlumniCircleBasicInfoDTO> queryAlumniCircleInfosFuzzilyByAluCirName(String name);

	/**
	 * 根据校友圈的名字来严格匹配查询查询.
	 *
	 * @param name 校友圈的名字.
	 * @return 模糊查询结果.
	 */
	public List<AlumniCircleBasicInfoDTO> queryAlumniCircleInfosByAluCirName(String name);

	public List<AlumniCircleBasicInfoDTO> alumniCirclesRecommend();

	public List<AlumniCircleMemberDTO> getAlumniCircleMembers(Long alumniCircleId,String query);

}
