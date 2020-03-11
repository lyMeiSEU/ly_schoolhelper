package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.FavoriteDTO;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.dao.entity.Favorite;
import cn.edu.seu.alumni_server.dao.mapper.FavoriteMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import tk.mybatis.mapper.entity.Example;

import javax.servlet.http.HttpServletRequest;
import java.util.List;
import java.util.Map;

/**
 * 名片收藏逻辑
 */
@RestController
@SuppressWarnings("ALL")
@Acl
public class FavoriteController {

    @Autowired
    FavoriteMapper favoriteMapper;
    @Autowired
    HttpServletRequest request;

    @GetMapping("/favorite")
    WebResponse getFavorite(@RequestParam int pageIndex,
                            @RequestParam int pageSize) {
        Long accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);

        Favorite favorite = new Favorite();
        favorite.setAccountId(accountId);
        favorite.setStatus(1);
        List<FavoriteDTO> res = favoriteMapper.getFavoriteList(accountId, pageIndex, pageSize);
        return new WebResponse().success(
                new PageResult<FavoriteDTO>(favoriteMapper.selectCount(favorite), res));
    }

    @PostMapping("/favorite")
    WebResponse changeFavoriteStatus(@RequestBody Map req) {
        Long accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);
        Long favoriteAccountId = (Long) req.get("favoriteAccountId");
        Integer status = (Integer) req.get("status");

        if (status != 0 && status != 1) {
            return new WebResponse().fail("status只能为0或1");
        }
        Favorite favorite = new Favorite();
        favorite.setAccountId(accountId);
        favorite.setFavoriteAccountId(favoriteAccountId);
        favorite.setStatus(status);
        if (favoriteMapper.select(favorite).size() == 0) {
            favoriteMapper.insertSelective(favorite);
        } else {
            Example example = new Example(Favorite.class);
            Example.Criteria criteria = example.createCriteria();
            criteria.andEqualTo("accountId", accountId);
            criteria.andEqualTo("favoriteAccountId", favoriteAccountId);

            favoriteMapper.updateByExampleSelective(favorite, example);
        }
        return new WebResponse();
    }
}
