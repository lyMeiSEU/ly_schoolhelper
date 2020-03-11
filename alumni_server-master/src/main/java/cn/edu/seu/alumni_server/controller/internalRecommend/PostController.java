package cn.edu.seu.alumni_server.controller.internalRecommend;

import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.controller.dto.internalRecommend.PostDTO4Create;
import cn.edu.seu.alumni_server.controller.dto.internalRecommend.PostDTO4Return;
import cn.edu.seu.alumni_server.dao.entity.Post;
import cn.edu.seu.alumni_server.dao.mapper.PostMapper;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

@RestController
@RequestMapping("/post")
public class PostController {

    @Autowired
    PostMapper postMapper;

    @Autowired
    HttpServletRequest request;

    @GetMapping("/")
    public WebResponse getPost(
            @RequestParam Long postId) {
        PostDTO4Return postDTO4Return = postMapper.getPost(postId);
//        Post post = postMapper.selectByPrimaryKey(postId);

        return new WebResponse().success(postDTO4Return);
    }


    @PostMapping("/")
    public WebResponse newPost(@RequestBody PostDTO4Create postDTO4Create) {
        Long accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);

        postDTO4Create.setPostId(Utils.generateId());
        postDTO4Create.setAccountId(accountId);

        // TODO ,直接新增，目前暂无修改操作
        postMapper.insertSelective(postDTO4Create.toPost());
        return new WebResponse().success();
    }

    @GetMapping("/list")
    public WebResponse getPosts(
            @RequestParam(required = false) Integer type,
            @RequestParam(required = false) String city,
            @RequestParam(required = false) String province,
            @RequestParam(required = false) String industry,
            @RequestParam int pageIndex,
            @RequestParam int pageSize) {
        Post post = new Post();
        post.setCity(city);
        post.setIndustry(industry);
        post.setProvince(province);
        post.setType(type);

        PageHelper.startPage(pageIndex, pageSize);
        List<PostDTO4Return> temp = postMapper.getPostList(post);

        return new WebResponse().success(
                new PageResult<>(((Page) temp).getTotal(), temp));
    }

}
