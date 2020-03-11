package cn.edu.seu.alumni_server.controller.baseInfo;

import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.ConstSchoolDTO;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.dao.entity.ConstSchool;
import cn.edu.seu.alumni_server.dao.mapper.ConstSchoolMapper;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import java.util.ArrayList;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Acl
public class ConstSchoolController {

	@Autowired
	ConstSchoolMapper constSchoolDao;

	@GetMapping("/queryConstSchool")
	public WebResponse queryConstSchool(@RequestParam String schoolName,
		@RequestParam int pageSize,
		@RequestParam int pageIndex) {
		PageHelper.startPage(pageIndex, pageSize);
		List<ConstSchool> temp = constSchoolDao.queryConstSchoolList(schoolName);
		List<ConstSchoolDTO> list = new ArrayList<>();
		temp.forEach(school -> {
			list.add(new ConstSchoolDTO(school));
		});
		return new WebResponse().success(
			new PageResult<>(((Page) temp).getTotal(), list)
		);
	}


}
