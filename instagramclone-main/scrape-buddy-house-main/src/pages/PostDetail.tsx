import { Navigation } from '@/components/Navigation';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { Button } from '@/components/ui/button';
import { Card } from '@/components/ui/card';
import { Separator } from '@/components/ui/separator';
import { Heart, MessageCircle, Send, Bookmark, MoreHorizontal, Smile } from 'lucide-react';
import { useState } from 'react';
import { Link, useParams } from 'react-router-dom';
import { posts } from '@/data/mockData';

const PostDetail = () => {
  const { id } = useParams();
  const post = posts.find((p) => p.id === id);
  const [liked, setLiked] = useState(false);
  const [likeCount, setLikeCount] = useState(post?.likes || 0);

  if (!post) {
    return (
      <div className="min-h-screen bg-background flex items-center justify-center">
        <p className="text-muted-foreground">Post not found</p>
      </div>
    );
  }

  const handleLike = () => {
    setLiked(!liked);
    setLikeCount(liked ? likeCount - 1 : likeCount + 1);
  };

  return (
    <div className="min-h-screen bg-background">
      <Navigation />
      
      <main className="max-w-5xl mx-auto pt-20 pb-10 px-4">
        <Card className="overflow-hidden border-border" data-post-id={post.id}>
          <div className="grid md:grid-cols-[1.2fr_1fr]">
            {/* Image Side */}
            <div className="bg-muted flex items-center justify-center">
              <img
                src={post.image}
                alt={post.caption}
                className="w-full h-full object-contain max-h-[80vh]"
              />
            </div>

            {/* Content Side */}
            <div className="flex flex-col">
              {/* Post Header */}
              <div className="p-4 flex items-center justify-between border-b border-border">
                <Link to={`/profile/${post.username}`} className="flex items-center gap-3 hover:opacity-80">
                  <Avatar className="h-9 w-9">
                    <AvatarImage src={post.userAvatar} alt={post.username} />
                    <AvatarFallback>{post.username[0].toUpperCase()}</AvatarFallback>
                  </Avatar>
                  <div>
                    <p className="text-sm font-semibold text-foreground">{post.username}</p>
                    {post.location && (
                      <p className="text-xs text-muted-foreground">{post.location}</p>
                    )}
                  </div>
                </Link>
                <Button variant="ghost" size="icon" className="h-8 w-8">
                  <MoreHorizontal className="h-5 w-5" />
                </Button>
              </div>

              {/* Comments Section */}
              <div className="flex-1 overflow-y-auto p-4 space-y-4">
                {/* Caption */}
                <div className="flex gap-3">
                  <Link to={`/profile/${post.username}`}>
                    <Avatar className="h-9 w-9">
                      <AvatarImage src={post.userAvatar} alt={post.username} />
                      <AvatarFallback>{post.username[0].toUpperCase()}</AvatarFallback>
                    </Avatar>
                  </Link>
                  <div className="flex-1">
                    <p className="text-sm">
                      <Link to={`/profile/${post.username}`} className="font-semibold hover:opacity-70">
                        {post.username}
                      </Link>{' '}
                      <span className="text-foreground">{post.caption}</span>
                    </p>
                    <p className="text-xs text-muted-foreground mt-1">{post.timestamp}</p>
                  </div>
                </div>

                <Separator />

                {/* Comments List */}
                {post.comments.map((comment) => (
                  <div key={comment.id} className="flex gap-3" data-comment-id={comment.id}>
                    <Link to={`/profile/${comment.username}`}>
                      <Avatar className="h-9 w-9">
                        <AvatarFallback>{comment.username[0].toUpperCase()}</AvatarFallback>
                      </Avatar>
                    </Link>
                    <div className="flex-1">
                      <p className="text-sm">
                        <Link to={`/profile/${comment.username}`} className="font-semibold hover:opacity-70">
                          {comment.username}
                        </Link>{' '}
                        <span className="text-foreground">{comment.text}</span>
                      </p>
                      <p className="text-xs text-muted-foreground mt-1">{comment.timestamp}</p>
                    </div>
                  </div>
                ))}
              </div>

              {/* Post Actions */}
              <div className="border-t border-border">
                <div className="p-4 flex items-center justify-between">
                  <div className="flex items-center gap-4">
                    <Button
                      variant="ghost"
                      size="icon"
                      className="h-8 w-8 hover:opacity-70"
                      onClick={handleLike}
                    >
                      <Heart
                        className={`h-6 w-6 transition-colors ${
                          liked ? 'fill-like-red text-like-red' : 'text-foreground'
                        }`}
                      />
                    </Button>
                    <Button variant="ghost" size="icon" className="h-8 w-8 hover:opacity-70">
                      <MessageCircle className="h-6 w-6" />
                    </Button>
                    <Button variant="ghost" size="icon" className="h-8 w-8 hover:opacity-70">
                      <Send className="h-6 w-6" />
                    </Button>
                  </div>
                  <Button variant="ghost" size="icon" className="h-8 w-8 hover:opacity-70">
                    <Bookmark className="h-6 w-6" />
                  </Button>
                </div>

                <div className="px-4 pb-2">
                  <p className="text-sm font-semibold text-foreground">
                    {likeCount.toLocaleString()} likes
                  </p>
                </div>

                {/* Add Comment */}
                <div className="p-4 border-t border-border flex items-center gap-3">
                  <Button variant="ghost" size="icon" className="h-8 w-8">
                    <Smile className="h-6 w-6" />
                  </Button>
                  <input
                    type="text"
                    placeholder="Add a comment..."
                    className="flex-1 bg-transparent outline-none text-sm text-foreground placeholder:text-muted-foreground"
                  />
                  <Button variant="ghost" size="sm" className="text-primary font-semibold">
                    Post
                  </Button>
                </div>
              </div>
            </div>
          </div>
        </Card>
      </main>
    </div>
  );
};

export default PostDetail;
