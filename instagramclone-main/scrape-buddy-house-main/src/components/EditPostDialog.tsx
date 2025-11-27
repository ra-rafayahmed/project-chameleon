import { useState, useEffect } from 'react';
import { Dialog, DialogContent, DialogHeader, DialogTitle } from '@/components/ui/dialog';
import { Button } from '@/components/ui/button';
import { Textarea } from '@/components/ui/textarea';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import type { Post } from '@/data/mockData';

interface EditPostDialogProps {
  post: Post | null;
  open: boolean;
  onOpenChange: (open: boolean) => void;
  onSave: (postId: string, updates: { caption: string; location?: string }) => void;
}

export const EditPostDialog = ({ post, open, onOpenChange, onSave }: EditPostDialogProps) => {
  const [caption, setCaption] = useState('');
  const [location, setLocation] = useState('');

  useEffect(() => {
    if (post) {
      setCaption(post.caption);
      setLocation(post.location || '');
    }
  }, [post]);

  const handleSave = () => {
    if (!post) return;
    
    onSave(post.id, {
      caption: caption.trim(),
      location: location.trim() || undefined,
    });
    
    onOpenChange(false);
  };

  if (!post) return null;

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-2xl">
        <DialogHeader>
          <DialogTitle>Edit Post</DialogTitle>
        </DialogHeader>

        <div className="space-y-4">
          {/* Post Image Preview */}
          <div className="relative w-full max-h-96 overflow-hidden rounded-lg">
            <img 
              src={post.image} 
              alt="Post preview" 
              className="w-full h-full object-contain bg-muted"
            />
          </div>

          {/* Caption */}
          <div className="space-y-2">
            <Label htmlFor="caption">Caption</Label>
            <Textarea
              id="caption"
              placeholder="Write a caption..."
              value={caption}
              onChange={(e) => setCaption(e.target.value)}
              rows={4}
              maxLength={2200}
            />
            <p className="text-xs text-muted-foreground text-right">
              {caption.length} / 2,200
            </p>
          </div>

          {/* Location */}
          <div className="space-y-2">
            <Label htmlFor="location">Location</Label>
            <Input
              id="location"
              placeholder="Add location (optional)"
              value={location}
              onChange={(e) => setLocation(e.target.value)}
              maxLength={100}
            />
          </div>

          {/* Actions */}
          <div className="flex justify-end gap-2">
            <Button 
              variant="outline" 
              onClick={() => onOpenChange(false)}
            >
              Cancel
            </Button>
            <Button onClick={handleSave}>
              Save Changes
            </Button>
          </div>
        </div>
      </DialogContent>
    </Dialog>
  );
};
