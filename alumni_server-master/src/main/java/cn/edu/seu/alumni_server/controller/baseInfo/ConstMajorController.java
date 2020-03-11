package cn.edu.seu.alumni_server.controller.baseInfo;

import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.ConstMajorDTO;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.dao.entity.ConstMajor;
import cn.edu.seu.alumni_server.dao.mapper.ConstMajorMapper;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;


@RestController
@Acl
public class ConstMajorController {
    @Autowired
    private ConstMajorMapper constMajorDao;

    @GetMapping("/queryConstMajor")
    public WebResponse queryConstMajor(@RequestParam String majorName,
                                       @RequestParam int pageSize,
                                       @RequestParam int pageIndex){
        PageHelper.startPage(pageIndex, pageSize);
        List<ConstMajor> temp = constMajorDao.queryConstMajorList(majorName);
        List<ConstMajorDTO> list = new ArrayList<>();
        temp.forEach(major -> {
            list.add(new ConstMajorDTO(major));
        });
        return new WebResponse().success(
                new PageResult<>(((Page)temp).getTotal(), list)
        );
    }


}
